# Ochre

## What is Ochre

Ochre is an [ABM](https://en.wikipedia.org/wiki/Agent-based_model) language focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) while keeping agent behavior code boilerplate-free.

Ochre also contains a simulation runtime that supports multithreaded simulation execution (which is done automatically because the code is race condition free), live coding (agent state data layouts get hot-reloaded as well as agent behavior code), and modularity of agent types so they can be easily reused in different models.

## How are race conditions relevant to ABM

Agent-based model state consists of individual agents' states and at each simulation step this state should change deterministically if no randomness is intentionally introduced.

During a simulation step each agent acts and interacts with, and in parallel to, other agents. Here "in parallel" just means that agents should *appear* as if they're running in parallel, the end result of their actions and interactions should be the same regardless of how their behavior is actually executed, and the actual execution can vary between running everything in a single thread or each agent running in its own thread.

Interactions between agents are done by agents directly reading from or writing into each other's memory. This of course raises the possibility of [data races](https://en.wikipedia.org/wiki/Race_condition#Data_race), and usually those are resolved by either not accessing memory directly (message passing), reading and writing using atomic operations, or just not executing anything in parallel at all. Either way, we introduce sequences into these reads and writes and get race conditions.

Following simple example shows how easy it is to get a data race and how different the results can be from the expected end state. Consider a row of cells whose state consists of only one variable containing either `+` or `-`. At each step each cell looks at its immediate neighbors and if either of them has `+` its state also becomes `+`.

```
# initial state
#   ----+----

foreach cell
    if cell.left_neighbor.state == '+' or cell.right_neighbor.state == '+'
        cell.state = '+'

# expected end state
#   ---+++---
# actual end state if going left to right
#   ---++++++
# actual end state if going right to left
#   ++++++---
```

This is the result of sequential reads and writes, and how different the results are depends on sequencing which, most importantly, is not under our control. The fact that those results are so different is just a symptom of race condition and that it's not enough to say that those two results are similar, because the difference between those results and expected results is enormous. And if we're making a more complex model we don't really know what the expected result is (or at least if the code we wrote really produces the expected result), we only can see that depending on the way a simulation is executed (most commonly by varying the number of threads) the results differ and that should ring some alarms.

#### Once we have sequencing we have RC

RW - double buffering
WW - always to the back buffer, and always accumulating, never just writing

#### Why double buffering?

Because we want the communication (accumulation in all the back buffers) to appear as if it happened all at the same time, instantaneously. The same with switching buffers. Here *appear* is the most iportant word, because at the point back buffer is being built, it is not visible by anyone, in fact everyone just sees the front buffer.
In short, whenever you want a complicated multi-step change to appear as instantaneous.

#### What is accumulation and why?

Accumulating the back buffer should be done using only [commutative](https://en.wikipedia.org/wiki/Commutative_property) operations (never their combinations): operations which produce a result that doesn't depend on their order.

If we say `a = Vi` and unroll that over the sequence:

```
a = V1
a = V2
a = V3
...
a = Vn
```

If we do `a += Vi` and unroll:

```
a = V1 + V2 + V3 + ... + Vn
```

that `a =` assignment is ok as long as it means that all of the Vi elements had the same chance of getting into the sum. Even if we do it conditionally:

```
if something < something_else
    a += Vi
```

as long as the condition only tests available **front** state it's OK.


```
// either of the following two can be used, but not both at the same time
acc += v # could be unrolled to acc = v1 + v2 + v3 + ... + vn, that's why the read-only and write-only rule still applies
acc *= v

# cannot be used
acc -= v
acc /= v

# special case that could work but requires checking whether an expression is commutative
acc = acc + v

# this works only if when iterating later we disregard the ordering, and we disregard ordering by not
# allowing random access and whenever we process elements we process all of them in the same way
collection.add(element)

# element is added if result of expression is minimal. There can be multiple minimal or maximal elements.
mincollection.add(element, element.x)

# accumulating conditionally is also OK
if something == true
    acc += 1
```

## Parallel execution

- Nice thing about solving race conditions first is that most data races disappear
- Once race conditions are eliminated parallelizing the simulation execution is trivial.
- Grid spatial partitioning, how bucketing for threads works and how the grid cells are actually infinite at its sides and edges.
1. How many ways can you double-buffer code? (there's at least Ochre's see and mod - what's the difference?)
2. After being double-buffered how does it parallelize?

#### Ochre example

`see` section for accumulating the back buffer, `act` section for switching buffers (voila, here's the new state!). Back buffer being `private` variables and front buffer `public` variables.

## Live coding

- Code *and* data hot-reload (link to Coo).

## Agent types modularity

- Each agent type in its own file, code describing interactions with agent types that are not loaded is ignored.
