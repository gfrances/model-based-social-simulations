
# Model-Based Social Simulations


A number of examples showcasing the effect of model-based decision-making procedures on social simulations.


## Dependencies

Besides the [requirements](https://github.com/xrubio/pandora/blob/master/docs/tutorials/00_installing.txt) imposed by the [Pandora framework](https://github.com/xrubio/pandora), 
In order for the build script to work, the path to the Pandora code and libraries must be provided, either through the `PANDORA_PATH`
environment variable, e.g. by setting the following in your `~/.bashrc` script:

	export PANDORA_PATH=${HOME}/projects/simulpast/pandora-main/pandora

or through a command line `pandora` argument when invoking scons, e.g:
	
	scons pandora=${HOME}/projects/simulpast/pandora-main/pandora
	