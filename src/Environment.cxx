
#include <Environment.hxx>

#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <Simulation.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <RasterLoader.hxx>

namespace Model 
{

Environment::Environment(const EnvironmentConfig & config, Engine::Simulation & simulation, Engine::Scheduler * scheduler ) : World(simulation, scheduler, false), _config(config)
{
}

Environment::~Environment()
{
}

void Environment::createRasters()
{
    registerDynamicRaster("resources", true);
    Engine::DynamicRaster & raster = getDynamicRaster("resources");
    Engine::GeneralState::rasterLoader().fillGDALRaster(raster, _config._fileName, getBoundaries());
	updateRasterToMaxValues("resources");
}

void Environment::createAgents()
{
	for(int i=0; i<_config._numAgents; i++)
	{
		if((i%getNumTasks())==getId())
		{
			std::ostringstream oss;
			oss << "ModelAgent_" << i;
			ModelAgent * agent = new ModelAgent(oss.str());
			addAgent(agent);
			agent->setRandomPosition();
		}
	}
}

} // namespace Model

