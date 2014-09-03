
#include <Environment.hxx>
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>
#include <Simulation.hxx>
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
		
        Model::EnvironmentConfig config;
		config.deserialize(configFile);
		
		omp_set_num_threads(2);
		
		// Initialize our logger
		Model::Logger::init(start_time, config.getLogDir());
		
	
		Engine::Simulation simParams(config.getSize(), config.getNumSteps(), config.getSerializeResolution());
        Model::Environment environment( config, simParams, environment.useOpenMPSingleNode(config.getResultsFile()));
	
		environment.initialize(argc, argv);
		environment.run();
	}
	catch( std::exception& e ){
		std::cout << "exception thrown: " << e.what() << std::endl;
	}
	return 0;
}

