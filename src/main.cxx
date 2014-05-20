
#include <Environment.hxx>
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>
#include <Simulation.hxx>

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
	try
	{	
		if(argc>2)
		{
			throw Engine::Exception("USAGE: modelSim [config file]");
		}		
	
		std::string fileName("config.xml");
		if(argc!=1)
		{
			fileName = argv[1];
		}
        Model::EnvironmentConfig config;
		config.deserialize(fileName);
	
		Engine::Simulation simParams(config.getSize(), config.getNumSteps(), config.getSerializeResolution());
        Model::Environment environment( config, simParams, environment.useOpenMPSingleNode(config.getResultsFile()));
	
		environment.initialize(argc, argv);
		environment.run();
	}
	catch( std::exception & exceptionThrown )
	{
		std::cout << "exception thrown: " << exceptionThrown.what() << std::endl;
	}
	return 0;
}

