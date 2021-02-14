# Ochre

Ochre is an [agent-based](https://en.wikipedia.org/wiki/Agent-based_model) modeling and simulation tool made for prototyping models and playing with simple systems. Primary goal of this tool is to build intuitive understanding of a complex system by shortening the modify-run cycle and focusing on the correlation between source code modifications and their effects on the running simulation.

![](res/ochre_particles.gif)

For modeling agent behavior Ochre incorporates a custom programming language focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) and keeping the code boilerplate-free (see how that was done [here](https://bcace.github.io/ochre.html)). Source code is compiled into bytcode which is interpreted on multiple threads (agent interactions and actions are distributed as evenly as possible among threads). To speed up the modify-run cycle changes to the source code are periodically compiled, and if there are no errors, new bytecode is injected into the running simulation. If agent data layout (its member variables) is also modified by these code changes all agents are updated in memory accordingly.

Agent definitions are modular in the sense that each agent type is implemented in its own file. These modules can then be freely mixed together into different models. Each agent type's module can contain code that defines interactions with other types, but if those other types are not currently loaded, that code is simply ignored.

This project was developed in 2013. and is not active anymore. I'm eventually going to give it another shot (Ochre itself is just the latest of at least four versions I developed from scratch since 2006.), but there are some elements of it that I'm trying to get right first, like generating machine code at run-time for code hot-reload that doesn't require an interpreter, [hot-reloading](https://github.com/bcace/coo) data layouts, efficient [space partitioning structures](https://bcace.github.io/tay.html), and even the [Boids](https://github.com/bcace/boids) project as an exercise in trying to make modeling a complex system more intuitive (even if this system is an airplane).
