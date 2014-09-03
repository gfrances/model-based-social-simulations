
#include <Environment.hxx>
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>
#include <utils/logging.hxx>
#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
	double start_time = MPI_Wtime();
	
	try {
		if(argc>2) throw Engine::Exception("USAGE: ./simulation.bin [config file]");
	
		std::string configFile(argc != 1 ? argv[1] : "config.xml");

		// Load the configuration file
        Model::EnvironmentConfig* config = new Model::EnvironmentConfig(configFile);
		config->loadFile();
		
		// Initialize our logger.
		Model::Logger::init(start_time, config->getLogDir());
		
		// omp_set_num_threads(2);
		// Initialize the simulation world.
        Model::Environment world(config, Model::Environment::useOpenMPSingleNode());
		world.initialize(argc, argv);
		world.run();
	}
	catch( std::exception& e ){
		std::cout << "exception thrown: " << e.what() << std::endl;
	}
	return 0;
}

