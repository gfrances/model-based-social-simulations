
#include <Environment.hxx>
#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <RasterLoader.hxx>
#include <utils/logging.hxx>

namespace Model 
{

Environment::Environment(EnvironmentConfig* config, Engine::Scheduler* scheduler) :
	World(config, scheduler, true), // We explicitly allow multiple agents per cell
	agentFactory()
{}

Environment::~Environment() {}

void Environment::createRasters() {
    registerDynamicRaster("resources", true, RESOURCE_RASTER_IDX);
    Engine::DynamicRaster& raster = getDynamicRaster(RESOURCE_RASTER_IDX);
    Engine::GeneralState::rasterLoader().fillGDALRaster(raster, getModelConfig().getMap(), getBoundaries());
	updateRasterToMaxValues(RESOURCE_RASTER_IDX);
}

void Environment::createAgents() {
	unsigned id = 1;
	for (const ControllerConfig& config:getModelConfig().getControllerConfigurations()) {
		agentFactory.registerControllerType(config);
		
		for(unsigned i = 0; i < config.getPopulation(); i++)
		{
			if((id % getNumTasks()) == (unsigned) getId()) {
				ModelAgent* agent = agentFactory.createAgent(id, config.getType());
				addAgent(agent);
				agent->setRandomPosition();
			}
			++id;
		}
	}
}

void Environment::addAgent(ModelAgent* agent) {
	Engine::World::addAgent(agent);
	PDEBUG("population", "[Timestep: " << getCurrentTimeStep() << "] Agent created: " << *agent);
}

void Environment::step() {
	// Ugly, but efficient
	#ifdef PANDORADEBUG
	logAgentsState();
	logMapState();
	#endif
	Engine::World::step();
}


const EnvironmentConfig& Environment::getModelConfig() const { return static_cast<const EnvironmentConfig&>(getConfig()); }

void Environment::logAgentsState() const {
	PDEBUG("agents", "");
	PDEBUG("agents", "*****************************************");
	PDEBUG("agents", "Agent overview at the beginning of timestep " << getCurrentTimeStep());
	for (auto agent:_agents) {
		PDEBUG("agents", *agent);
	}
	PDEBUG("map", "");
}

void Environment::logMapState() const {
	const Engine::DynamicRaster& raster = getDynamicRaster(RESOURCE_RASTER_IDX);
	PDEBUG("map", "");
	PDEBUG("map", "*****************************************");
	PDEBUG("map", "Map resources at the beginning of timestep " << getCurrentTimeStep());
	for (auto point:getBoundaries()) {
		PDEBUG("map", "(" << point._x << "," << point._y << "): " << raster.getValue(point) << "/" << raster.getMaxValue(point));
	}
	PDEBUG("map", "");
	
}

} // namespace Model

