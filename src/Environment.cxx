
#include <Environment.hxx>
#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <RasterLoader.hxx>
#include <utils/logging.hxx>
#include <utils/memory.hxx>

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
    Engine::GeneralState::rasterLoader().fillGDALRaster(raster, getModelConfig()->getMap(), getBoundaries());
	updateRasterToMaxValues(RESOURCE_RASTER_IDX);
}

void Environment::createAgents() {
	const auto modelConfig = getModelConfig();
	unsigned id = 1;
	for (const ControllerConfig& config:modelConfig->getControllerConfigurations()) {
		agentFactory.registerControllerType(config);
		
		for(unsigned i = 0; i < config.getPopulation(); i++)
		{
			if((id % getNumTasks()) == (unsigned) getId()) {
				ModelAgent* agent = agentFactory.createAgent(id, this, config.getType());
				addAgent(agent);
				
				// Set the agent initial position
				const auto position = modelConfig->getInitialAgentPosition();
				if (position) {
					agent->setPosition(*position);
				} else { // If no fixed position was specified in the config file, we set a random position.
					agent->setRandomPosition();
				}
			}
			++id;
		}
	}
}

void Environment::addAgent(ModelAgent* agent, bool executedAgent) {
	Engine::World::addAgent(agent, executedAgent);
	PDEBUG("population", "[Timestep: " << getCurrentTimeStep() << "] Agent created: " << *agent);
}

// We only need to log the event.
void Environment::removeAgent(ModelAgent* agent) {
	PDEBUG("population", "[Timestep: " << getCurrentTimeStep() << "] Agent dead: " << *agent);
}


void Environment::step() {
	// Ugly, but efficient
	#ifdef PANDORADEBUG
	logAgentsState();
	logMapState();
	#endif
	Engine::World::step();
	
	// Log memory usage
	PINFO("memory", "[Timestep: " << getCurrentTimeStep() << "] Current / Peak: " << getCurrentRSS() << " / " << getPeakRSS());
}


const EnvironmentConfig::cptr Environment::getModelConfig() const { return std::static_pointer_cast<EnvironmentConfig>(_config); }

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

