# Ochre

Ochre is an [agent-based](https://en.wikipedia.org/wiki/Agent-based_model) modeling and simulation tool made for prototyping models and playing with simple systems. Primary goal of this tool is to build intuitive understanding of a complex system by shortening the modify-run cycle and focusing on the correlation between source code modifications and their effects on the running simulation. This is done putting source code and 3D view of the running simulation side-by-side, and hot-reloading code of the running simulation.

![](res/ochre_particles.gif)

For modeling agent behavior it incorporates a custom programming language focused on eliminating [race conditions](https://en.wikipedia.org/wiki/Race_condition) and keeping the code boilerplate-free (see how that was done [here](https://bcace.github.io/ochre.html)).
