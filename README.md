# Ochre

Ochre is an [ABM](https://en.wikipedia.org/wiki/Agent-based_model) language focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) while keeping agent behavior code boilerplate-free. Ochre simulation runtime supports multithreaded simulation execution, live coding, and agent type modularity.

## Race conditions in ABM

At each simulation step agents interact with each other, and each of those interactions is a parallel logical process that changes agent states. Since we want to keep the simulation deterministic varying the number of threads or physical processes in which those logical processes are executed should not change the simulation results or the sequence of states the model goes through.

Most efficient interaction between agents is direct exchange of data in shared memory, which introduces the possibility of [data races](https://en.wikipedia.org/wiki/Race_condition#Data_race), and data races are usually resolved by either not accessing memory directly (message passing), reading and writing using atomic operations, or simply executing everything sequentially. Either way, we solve data races by sequencing reads and writes and transform the problem from data races into race conditions.

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

So after we double buffered the variables we can still see that the innermost statement of the "back" buffer building pass contains an assignment, which means that we still have the problem of writing to the same memory from different logical processes.

Writing to same memory form different logical processes can be solved by accumulation - when "back" data is being built we have to make sure that operations building it are [commutative](https://en.wikipedia.org/wiki/Commutative_property), in other words we can only use operations that produce the same result regardless of the ordering of their operands. In the above code `or` operator is commutative...

Similarly we could write either `a += Ai` or `a *= Ai` but we cannot mix them, so:

```
a = A1 + A2 + A3 + ... + An
b = B1 * B2 * B3 * ... * Bn
```

works but `a += Ai; a *= Bi` doesn't because unrolled it would look like this:

```
a = (((A1 * B1 + A2) * B2 + A3) * B3 + ... + An) * Bn
```

does not. Obviously `a -= Ai` would not work because `-` is not commutative. Even collections work if we limit how they're updated and how their elements are inspected. When building a collection as part of the "back" buffer we can only allow adding elements to the collection. Then, when reading it as part of the "front" buffer we only have to ensure that whatever we want to do with its elements we do *equally* to *all* elements, no random access to specific elements.

## Ochre spin on the above

As mentioned at the beginning these rules for eliminating race conditions have to be somehow turned into a concise programming language that doesn't require struggling through parallel reads and writes at every line of code and at the same time be flexible enough to allow expressing most forms of agent action and interaction in an intuitive way. As a bonus it should generate executable code that can be safely executed on multiple threads.

First some general things you need to know about the language and its runtime before we go into more details:
* There are no explicit loops to process agents and their interactions, you write code acting on a single agent or pairs of agents and runtime decides how to use the code and when.
* Interaction sections of agent behavior code always describe interactions between two agents provided by the runtime through `this` and `other` references. Interaction sections are written from the perspective of the `this` agent and in most cases it can be omitted.
* Interaction can be defined for cases when agents are close enough to each other (proximity), or when we want all interactions to go through references (direct links between agents).
* Proximity is a global property of the simulation environment and can be changed at run-time.

Each agent type is defined in its own file, first non-comment line contains the type name, and the rest of the file is divided into *sections*:

* declaration section: space between agent type name and the next section, used to declare agent (instance) and type (static) variables. For double buffering purposes variables are separated into *front*, *back* and *constant* variables. Variable is declared as a *back* variable if it starts with a lowercase letter, and *front* variable if it starts with an uppercase letter. Each of the following sections have their own set of rules on how all these types of variables can be used.

```
A # agent type name

int i # agent's "back" variable
int I # agent's "front" variable
```

* `set` section: used to create and initialize agents. In this section there are no limitations on how variables are used.

* `see` section: used to define how agents "see" their environment. Multiple `see` sections can be defined, depending on which agent type the current agent type should interact and whether they should interact only if close enough or only if they have a direct link.

```
# agents of type A "see" other agents of the same type, if they're closer than the interaction distance
see A
    i += other.I

# agents of type A "see" agents of type B, if they're closer than the interaction distance
see B
    i += other.J

# agents of type A "see" agents contained in the "neighbors" agent references collection
see neighbors
    i += other.K
```

| | front | back
| --- | --- | ---
| this | read | accumulate
| other | read | -
| type | read | read
| local | read and write | read and write

* `mod`

* `act`

* `do`

An agent type can have multiple interaction (`see` and `mod`) sections defined, one for each type it has to interact with. If a type is not currently loaded
- agent type oriented
- each type defines blocks of code that gets executed by the runtime at certain times and

```
see neighbors
    if other.is_plus
        neighbors_with_pluses += 1

act
    if neighbors_with_pluses > 0
        is_plus = 1
```

#### Parallel execution

- Grid spatial partitioning, how bucketing for threads works and how the grid cells are actually infinite at its sides and edges.
1. How many ways can you double-buffer code? (there's at least Ochre's see and mod - what's the difference?)
2. After being double-buffered how does it parallelize?

`see` section for accumulating the back buffer, `act` section for switching buffers (voila, here's the new state!). Back buffer being `private` variables and front buffer `public` variables.

#### Proximity and connections

#### Modularity of agent types in Ochre


#### Live coding

- Code *and* data hot-reload (link to Coo).
