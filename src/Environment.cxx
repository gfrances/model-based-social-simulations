
#include <Environment.hxx>

#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <AgentFactory.hxx>
#include <Simulation.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <RasterLoader.hxx>

namespace Model 
{
	
Environment::Environment(const EnvironmentConfig & config, Engine::Simulation & simulation, Engine::Scheduler * scheduler ) :
	World(simulation, scheduler, true), // We explicitly allow multiple agents per cell
	_config(config)
{}

Environment::~Environment() {}

void Environment::createRasters() {
    registerDynamicRaster("resources", true, RESOURCE_RASTER_IDX);
    Engine::DynamicRaster & raster = getDynamicRaster(RESOURCE_RASTER_IDX);
    Engine::GeneralState::rasterLoader().fillGDALRaster(raster, _config._fileName, getBoundaries());
	updateRasterToMaxValues(RESOURCE_RASTER_IDX);
}

void Environment::createAgents() {
	auto agentFactory = AgentFactory(_config.getControllerConfig());
	for(unsigned i = 0; i < _config._numAgents; i++)
	{
		if((i % getNumTasks()) == (unsigned) getId()) {
			ModelAgent* agent = agentFactory.createAgent(i);
			addAgent(agent);
			agent->setRandomPosition();
		}
	}
}

ModelAgent* Environment::createAgent(const std::string id, const Engine::Point2D<int>& position) {
	auto agentFactory = AgentFactory(_config.getControllerConfig());
	ModelAgent* agent = agentFactory.createAgent(id);
	addAgent(agent);
	agent->setPosition(position);
	return agent;
}

} // namespace Model

