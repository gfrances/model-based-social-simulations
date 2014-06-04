
# Model-Based Social Simulations


A number of examples showcasing the effect of model-based decision-making procedures on social simulations.


## Simulation Parameters
A number of simulation parameters can be specified in the `config.xml` file, that currenty include:

* `size`: Size of the map, in number of cells.
* `environment`: File with the initial distribution of the resources across the map cells.
* `numSteps`: Number of time steps that the simulation will run.
* `agents`: Initial number of agents in the simulation.

## Dependencies

Besides the [requirements](https://github.com/xrubio/pandora/blob/master/docs/tutorials/00_installing.txt) imposed by the [Pandora framework](https://github.com/xrubio/pandora), 
In order for the build script to work, the path to the Pandora code and libraries must be provided, either through the `PANDORA_PATH`
environment variable, e.g. by setting the following in your `~/.bashrc` script:

	export PANDORA_PATH=${HOME}/projects/simulpast/pandora-main/pandora

or through a command line `pandora` argument when invoking scons, e.g:
	
	scons pandora=${HOME}/projects/simulpast/pandora-main/pandora
	