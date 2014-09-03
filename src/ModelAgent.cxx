
#include "ModelAgent.hxx"
#include <Statistics.hxx>
#include <World.hxx>
#include <GeneralState.hxx>
#include <Environment.hxx>
#include <utils/logging.hxx>

namespace Model
{

ModelAgent::ModelAgent(unsigned id, std::shared_ptr<AgentController> controller)
	: ModelAgent("ModelAgent_" + std::to_string(id), controller)
{}

ModelAgent::ModelAgent(const std::string& id, std::shared_ptr<AgentController> controller)
	: Agent(id), _controller(controller), _resources(INITIAL_RESOURCES), _numChildren(0)
{}


ModelAgent::~ModelAgent() {}

void ModelAgent::selectActions() {
	assert(_actions.size()==0);
	// We select one single action, and the logic of selection is embedded in the particular instance
	// of the controller
	_actions.push_back(_controller->selectAction(*this)); 
}

void ModelAgent::updateState() {
    
	// Collect the resources from the map
	_resources += collectResources(getResourceRaster(), getPosition()); // Collect the resources
	
	// Consume the resources
	_resources = consumeDailyResources(_resources);
	
	// Check for the death of the agent
    if (checkDeathByStarvation(_resources)) {
		remove();
	}
	
	// Check for the reproduction of the agent
    if (checkReproduction(_resources)) {
		reproduceAgent();
	}
}

void ModelAgent::reproduceAgent() {
	// Create an additional agent with a subordinated ID
	++_numChildren;
	std::string id = getId() + "." + std::to_string(_numChildren);
	static_cast<Environment *>(getWorld())->createAgent(id, getPosition());
	
	// And reduce the number of resources of the current agent
	_resources = consumeResourcesOnReproduction(_resources);
	PDEBUG("population", getId() << " gives birth to agent " << id);
}


int ModelAgent::collectResources(Engine::DynamicRaster& resourceRaster, const Engine::Point2D<int>& position) {
	// The agent forages a certain stochastic amount of resources from her destination cell:
	// The amount is drawn uniformly at random between 1 and the total amount of resources available on the cell.
	int availableResources = resourceRaster.getValue(position);
	
	if (availableResources == 0) return 0;
	
	// A small performance optimization to avoid invoking the Random Number Generator if there is only one unit of resources.
	int collectedResources = (availableResources == 1) ? 1 : Engine::GeneralState::statistics().getUniformDistValue(1, availableResources); 
	resourceRaster.setValue(position, availableResources - collectedResources);
	return collectedResources;
}


void ModelAgent::registerAttributes() {
	registerIntAttribute("resources");
	registerIntAttribute("numChildren");
}

void ModelAgent::serialize() {
	serializeAttribute("resources", _resources);
	serializeAttribute("numChildren", _numChildren);
}

//! Helpers to return the agent's world's resource raster, const- and non-const versions
const Engine::DynamicRaster& ModelAgent::getResourceRaster() const {
	return getWorld()->getDynamicRaster(Environment::RESOURCE_RASTER_IDX);
}
Engine::DynamicRaster& ModelAgent::getResourceRaster() {
	return getWorld()->getDynamicRaster(Environment::RESOURCE_RASTER_IDX);
}

std::ostream& ModelAgent::print(std::ostream& os) const {
	os << getId();
	if (!exists()) os << " [DEAD!]";
	if (!getWorld()) os << " [DETACHED!]";
	
	os << " [@" << "(" << _position._x << "," << _position._y << ")" << "]";
	os << " [res: " << getResources() << "]";
	return os;
}

} // namespace Model

