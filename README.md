
# Model-Based Social Simulations


A number of examples showcasing the effect of model-based decision-making procedures on social simulations.


## Getting Started
The simulation parameters must be placed in `src/config.xml`. A sample configuration file is provided in `src/config.xml.sample`.
To compile the simulation engine, `cd src && scons`.

## Dependencies

Besides the [requirements](https://github.com/xrubio/pandora/blob/master/docs/tutorials/00_installing.txt) imposed by the [Pandora framework](https://github.com/xrubio/pandora), 
In order for the build script to work, the path to the Pandora code and libraries must be provided, either through the `PANDORAPATH`
environment variable, e.g. by setting the following in your `~/.bashrc` script:

	export PANDORAPATH=${HOME}/projects/simulpast/pandora-main
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${PANDORAPATH}/lib


	
