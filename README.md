
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
In order for the build script to work, the path to the Pandora code and libraries must be provided, either through the `PANDORAPATH`
environment variable, e.g. by setting the following in your `~/.bashrc` script:

	export PANDORAPATH=${HOME}/projects/simulpast/pandora-main
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PANDORAPATH}/lib


	
