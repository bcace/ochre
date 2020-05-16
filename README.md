# Ochre

Ochre is an [ABM](https://en.wikipedia.org/wiki/Agent-based_model) language focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) while keeping agent behavior code boilerplate-free. Ochre simulation runtime supports multithreaded simulation execution, live coding, and agent type modularity.

## Race conditions in ABM

At each simulation step agents interact with each other, and each of those interactions is a parallel logical process during which agent state is changed. Since we want to keep the simulation deterministic, varying the number of threads or physical processes in which those logical processes are executed should not change the simulation results or the sequence of states the model goes through.

Most efficient interaction between agents is by direct exchange of data in shared memory, which introduces the possibility of [data races](https://en.wikipedia.org/wiki/Race_condition#Data_race), and data races are usually resolved by either not accessing memory directly (message passing), reading and writing using atomic operations, or simply executing everything sequentially. Either way, we solve data races by sequencing reads and writes and transform the problem from data races into race conditions.

The following example shows how easy it is to get a race condition and how different the end state is from what we expect. Consider a row of cells whose state consists of only one variable containing either `+` or `-`. At each step each cell looks at its immediate neighbors and if either one of them has `+` its state also becomes `+`.

```
# initial state
#   ----+----
# expected end state
#   ---+++---

foreach cell in cells
    if cell.left_neighbor.state == '+' or cell.right_neighbor.state == '+'
        cell.state = '+'

# actual end state
#   ---++++++  # left to right
#   ++++++---  # right to left
```

How these interactions are sequenced is either not under our control (e.g. thread scheduler controls thread execution), or in case of single threaded execution, where we could enforce consistent sequencing, we could get consistent results but still have race conditions because the sequencing we forced is not tied semantically to the model.

Also, the differences between results we get with race conditions could relatively small, but the very fact they exist could be a sign that if we fix race conditions results could be vastly different. So in this simple example we know what the expected result is and we can easily evaluate the result we get, but in any sufficiently complex model we either don't know what the expected result is, or we only *believe* that the code we wrote produced expected results in a correct way. In these cases we should take the inconsistency of simulation results when running on different number of threads as a symptom of a possible race condition.

### Untangling reads and writes

If we assume that all reads and writes are sequential we just have to eliminate the *effects* of sequential reads and writes between parallel logical processes.

#### Read/write

The problem of reading from and writing to same memory we can solve by double-buffering - agents can read each other's "front" data while their "back" data is being built. In the above example that would mean we have to split the `state` variable into "back" and "front" parts. We only read from the "front" variable and only write to the "back" variable:

```
foreach cell in cells
    if cell.left_neighbor.front_state == '+' or cell.right_neighbor.front_state == '+'
        cell.back_state = '+'
```

and after all "back" values are updated, "front" values can be updated from them (buffer swap):

```
foreach cell in cells
    cell.front_state = cell.back_state
```

#### Write/write

So after we double-buffered the variables we can still see that the innermost statement of the "back" buffer building pass contains an assignment, which means that we still have the problem of writing to the same memory from different logical processes.

Writing to same memory form different logical processes can be solved by accumulation - when "back" data is being built we have to make sure that operations building it are [commutative](https://en.wikipedia.org/wiki/Commutative_property), in other words we can only use operations that produce the same result regardless of the ordering of their operands. In the above code `or` operator is commutative...

Similarly we could write either `a += Ai` or `a *= Ai` but we cannot mix them, so:

```
a = A1 + A2 + A3 + ... + An
b = B1 * B2 * B3 * ... * Bn
```

works but `a += Ai` and `a *= Bi` doesn't because unrolled it would look like this:

```
a = (((A1 * B1 + A2) * B2 + A3) * B3 + ... + An) * Bn
```

does not. Obviously `a -= Ai` would not work because `-` is not commutative. Even collections work if we limit how they're updated and how their elements are inspected. When building a collection as part of the "back" buffer we can only allow adding elements to the collection. Then, when reading it as part of the "front" buffer we only have to ensure that whatever we want to do with its elements we do *equally* to *all* elements, no random access to specific elements.

## Concurrency safety in Ochre

The purpose of Ochre as a language is to ensure concurrency safety with a set of high-level rules based on double-buffering and accumulation. These rules should be easy to comprehend while still being flexible enough for expressing various forms of agent interactions. For semantic analysis based on these rules the following information must be known:
    1. which code belongs to which double-buffering phase,
    2. which memory (agent variables) belongs to which buffer (front/back),
    3. how operators and functions interact with data in terms of reading, writing and accumulation.

1. Simulation steps are split into interaction and action phases which correspond to the two double-buffering phases (accumulation and swap). Agent behavior code is written for specific phases, and all loops are implicit, allowing the Ochre runtime to distribute work among worker threads and synchronize them.

2. All agent variables are declared as *front* or *back* variables, and each phase has its own set of rules how these variables can be used: if they can be read, written to or only accumulated.

3. Each operator and function is annotated with information on how it uses its arguments and how its result can be used: read, write or accumulate.

#### Anatomy of an Ochre agent type

{{{ a description of what this language actually looks like, very simplified, lots of stuff missing for simplicity }}}

Each agent type is defined in its own file, and the file is divided into sections. First section contains the Type name and declaration of agent and type variables. Agent variable is a *back* variable if the first character is lowercase, and a *front* variable if uppercase.

```
TypeA # first non-comment line is the type name

    # declarations of agent variables
    bool backVariable
    int FrontVariable
```

**Interaction sections**

Multiple interaction sections can be defined, but they are all part of the simulation step phase where back buffers are accumulated. Interaction sections are just blocks of code, and they act on pairs of agents: `this` and `other` (`this` reference can be omitted (implicit)). Interaction code is always written from the perspective of the `this` agent, and it will be executed for all agents of the type.

```
see TypeA
    # agent counts neighbors that are closer than 10
    if (Position - other.Position).length() < 10
        neighborsCount += 1
```

In this snippet *front* variables (`[this.]Position` and `other.Position`) are being read and the *back* `[this.]neighborsCount` variable is accumulated, which conforms to both double-buffering and accumulation rules. Ochre rules for agent variables in `see` sections are:

| | front | back
| --- | --- | ---
| this | read | accumulate
| other | read | -

Which agents are going to be the `other` agent in that section is determined by the section statement argument (`see TypeA`). If type name is used, any agent of that type close enough to `this` agent will be the `other` agent. How close the agents have to be is determined by the size of the environment grid cell size. Agents can interact with other agents in the same cell or the neighboring cells.

```
# interaction with agents of the same type
see TypeA
    # "this" will be a reference to a TypeA agent
    # "other" will be a reference to a different TypeA agent

# interaction with agents of type TypeB
# ignored if TypeB is not loaded into the Ochre environment
see TypeB
    # "this" will be a reference to a TypeA agent
    # "other" will be a reference to a TypeB agent
```

Another way of specifying which agents will interact is through direct references. Agents can have collections of agent references as its variables, and interaction section is declared by just passing the variable name as the `see` statement argument:

```
see collectionOfAgents
    # "this" will be a reference to a TypeA agent
    # "other" will be a reference to one of the agents in collectionOfAgents
```

**Action section**

An agent type can have only one action phase section (`act`) and it's simply a block of code that's executed for each agent, and the Ochre runtime decides how to distribute agents among worker threads.

```
act
    # at each simulation step each agent moves by 1 along x axis
    P.x += 1
```

**Grid**

...
