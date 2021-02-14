# Ochre

Ochre is an [agent-based](https://en.wikipedia.org/wiki/Agent-based_model) modeling and simulation tool made for prototyping models and playing with simple systems. Primary goal of this tool is to build intuitive understanding of a complex system by shortening the modify-run cycle and focusing on the correlation between source code modifications and their effects on the running simulation. This is done putting source code and 3D view of the running simulation side-by-side, and hot-reloading code of the running simulation.

![](res/ochre_particles.gif)

For modeling agent behavior it incorporates a custom programming language focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) and keeping the code boilerplate-free (see how that was done [here](https://bcace.github.io/ochre.html)).

This project was developed in 2013. and I'm not actively developing it. I'm most probably going to give it another shot (Ochre itself is just the latest of at least four versions I developed from scratch since 2006.), but there are some elements of it that I'm trying to get right first, like generating machine code at run-time for code hot-reload that doesn't require an interpreter, [hot-reloading](https://github.com/bcace/coo) data layouts, efficient [space partitioning structures](https://bcace.github.io/tay.html), and even [boids](https://github.com/bcace/boids) as an exercise in trying to make modeling a complex system more intuitive, even if this system is an airplane.
