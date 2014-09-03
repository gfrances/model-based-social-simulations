
#include <Environment.hxx>

#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <AgentFactory.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <RasterLoader.hxx>

namespace Model 
{

Environment::Environment(EnvironmentConfig* config, Engine::Scheduler* scheduler) :
	World(config, scheduler, true) // We explicitly allow multiple agents per cell
{}

Environment::~Environment() {}

void Environment::createRasters() {
    registerDynamicRaster("resources", true, RESOURCE_RASTER_IDX);
    Engine::DynamicRaster & raster = getDynamicRaster(RESOURCE_RASTER_IDX);
    Engine::GeneralState::rasterLoader().fillGDALRaster(raster, getModelConfig().map, getBoundaries());
	updateRasterToMaxValues(RESOURCE_RASTER_IDX);
}

void Environment::createAgents() {
	auto agentFactory = AgentFactory(getModelConfig().getControllerConfig());
	for(unsigned i = 0; i < getModelConfig()._numAgents; i++)
	{
		if((i % getNumTasks()) == (unsigned) getId()) {
			ModelAgent* agent = agentFactory.createAgent(i);
			addAgent(agent);
			agent->setRandomPosition();
		}
	}
}

ModelAgent* Environment::createAgent(const std::string id, const Engine::Point2D<int>& position) {
	auto agentFactory = AgentFactory(getModelConfig().getControllerConfig());
	ModelAgent* agent = agentFactory.createAgent(id);
	addAgent(agent);
	agent->setPosition(position);
	return agent;
}

const EnvironmentConfig& Environment::getModelConfig() const { return static_cast<const EnvironmentConfig&>(getConfig()); }


} // namespace Model

