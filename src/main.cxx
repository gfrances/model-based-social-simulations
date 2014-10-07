
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
	
	if(argc>2) throw Engine::Exception("USAGE: ./simulation.bin [config file]");

	std::string configFile(argc != 1 ? argv[1] : "config.xml");

	// Load the configuration file
	Model::EnvironmentConfig* config = new Model::EnvironmentConfig(configFile);
	
	// omp_set_num_threads(2);
	
	// Initialize the simulation world.
	Model::Environment world(config, Model::Environment::useOpenMPSingleNode());
	
	// Initialize our logger.
	Model::Logger::init(start_time, config->getLogDir()); // This needs to be after the creation of the world, otherwise the configuration is still not loaded.
	
	world.initialize(argc, argv);
	world.run();

	return 0;
}

