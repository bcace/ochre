# Ochre

## What is Ochre

Ochre is an [ABM](https://en.wikipedia.org/wiki/Agent-based_model) language focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) while keeping agent behavior code boilerplate-free.

Ochre also contains a simulation runtime that supports multithreaded simulation execution (which is done automatically because the code is race condition free), live coding (agent state data layouts get hot-reloaded as well as agent behavior code), and modularity of agent types so they can be easily reused in different models.

## What are race conditions and how are they relevant to ABM

- Simulation iterations.
- Model state is made up of agent states.
- Model state should change from t1 to t2 deterministically.
- Each agent acts in parallel to the others.
- Agents communicate by means of shared memory, communication means reading and writing memory directly, one agent can read or write other agent's state.
- There can be no effects of ordering of operations on how model transitions from one state to the next.

Wavefront example:

```
# initial state:
# oooxooo
# expected end state after first step:
# ooxxxoo

foreach agent
    if agent.left.value == X or agent.right.value == X
        agent.value = X

# end state if going left to right
# ooxxxxx

# end state if going right to left
# xxxxxoo
```

This is the result of sequential reads and writes, and how different the results are depends on sequencing which, most importantly, is not under our control. The fact that those results are so different is just a symptom of race condition and that it's not enough to say that those two results are similar, because the difference between those results and expected results is enormous. And if we're making a more complex model we don't really know what the expected result is (or at least if the code we wrote really produces the expected result), we only can see that depending on the way a simulation is executed (most commonly by varying the number of threads) the results differ and that should ring some alarms.

^^ should mention random.

#### How does Ochre approach this problem

#### If we assume there are not data races

[data races](https://en.wikipedia.org/wiki/Race_condition#Data_race)
Start with assuming that all data races are fixed. How things are scheduled to achieve this doesn't matter, the only thing is that two writes to the same memory never happen at the same time, and one read and one write don't happen at the same time. Doesn't matter how, it's clear that this has to happen.
Either because everything is executed on a single processor, or there's mutexes.

#### From that we get sequential WW and RW

From that we know that there's sequencing issues, and it's exactly this sequencing that cannot have any effect on the final model state.
Why? Probably because this sequencing doesn't have anything to do with time. You could schedule things, and have complete control over sequencing, but what value in the model itself do you tie this sequencing to?

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

# either of the following two can be used, but not both at the same time
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

#### Ochre example

`see` section for accumulating the back buffer, `act` section for switching buffers (voila, here's the new state!). Back buffer being `private` variables and front buffer `public` variables.

## Parallel execution

- Once race conditions are eliminated parallelizing the simulation execution is trivial.
- Grid spatial partitioning, how bucketing for threads works and how the grid cells are actually infinite at its sides and edges.
1. How many ways can you double-buffer code? (there's at least Ochre's see and mod - what's the difference?)
2. After being double-buffered how does it parallelize?

## Live coding

- Code *and* data hot-reload (link to Coo).

## Agent types modularity

- Each agent type in its own file, code describing interactions with agent types that are not loaded is ignored.
