# Ochre

## What is Ochre?

Ochre is an ABM language and runtime focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) while keeping the agent behavior free of simulation boilerplate code.

## What are race conditions and how are they relevant to ABM

- Simulation iterations.
- Model state is made up of agent states.
- Model state should change from t1 to t2 deterministically.
- Each agent acts in parallel to the others.
- LPs communicate by means of shared memory, communication means reading and writing memory directly, one agent can read or write other agent's state.
- There can be no effects of ordering of operations on how model transitions from one state to the next.

Data races don't exist
    Start with assuming that all data races are fixed. How things are scheduled to achieve this doesn't matter, the only thing is that two writes to the same memory never happen at the same time, and one read and one write don't happen at the same time. Doesn't matter how, it's clear that this has to happen.
    Either because everything is executed on a single processor, or there's mutexes.
From that we get sequential WW and RW
    From that we know that there's sequencing issues, and it's exactly this sequencing that cannot have any effect on the final model state.
    Why? Probably because this sequencing doesn't have anything to do with time. You could schedule things, and have complete control over sequencing, but what value in the model itself do you tie this sequencing to?
Once we have sequencing we have RC
    RW - double buffering
    WW - always to the back buffer, and always accumulating, never just writing
Why double buffering?
    Because we want the communication (accumulation in all the back buffers) to appear as if it happened all at the same time, instantaneously. The same with switching buffers. Here *appear* is the most iportant word, because at the point back buffer is being built, it is not visible by anyone, in fact everyone just sees the front buffer.
    In short, whenever you want a complicated multi-step change to appear as instantaneous.
What is accumulation and why?
    a += 1
    b *= 3
    collection.append(element) # only if when iterating later we disregard the ordering, Ochre does that.
    Because if we do a = 2 we overwrite the old value, revealing the sequence.

## Ochre example
    `see` section for accumulating the back buffer, `act` section for switching buffers (voila, here's the new state!). Back buffer being `private` variables and front buffer `public` variables.

## Parallel execution

- Once race conditions are eliminated parallelizing the simulation execution is trivial.
- Grid spatial partitioning, how bucketing for threads works and how the grid cells are actually infinite at its sides and edges.

## Live coding

- Code *and* data hot-reload (link to Coo).

## Agent type modularity

- Each agent type in its own file, code describing interactions with agent types that are not loaded is ignored.
