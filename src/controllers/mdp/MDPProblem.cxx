
#include "MDPProblem.hxx"

#include <Exception.hxx>
#include <Logger.hxx>
#include <EnvironmentConfig.hxx>
#include <limits>

using Problem::action_t;

namespace Model
{

MDPProblem::MDPProblem(const ControllerConfig& config, const ModelAgent& agent) :
	_config(config),
	_agent(agent),
	initialState(agent.getPosition(), agent.getResourceRaster(), agent.getResources(),  MoveAction::computeApplicableActions(agent))
{}

MDPProblem::~MDPProblem() {}

action_t MDPProblem::number_actions(const MDPState& s) const {
	return s.getApplicableActions().size();
}

bool MDPProblem::applicable( const MDPState& s, action_t a ) const { return true; }


float MDPProblem::cost( const MDPState& s, action_t a ) const {
	// TODO - Note that this is slightly inefficient and redundant, since this method is always called after MDPProblem::next,
	// which has already performed the same operations. 
	
	MoveAction::cptr action = s.getApplicableActions().at(a);
	Engine::Point2D<int> position = action->getResultingPosition(s.getAgentPosition());
	int availableResources = s.getResourceRaster().getValue(position);
	
	int maximumAttainable = s.getAgentResources() + availableResources;
	return maximumAttainable > 0 ? 10.0 / (float) maximumAttainable : 100;
}

void MDPProblem::next(const MDPState& s, action_t a, OutcomeVector& outcomes) const {
	assert(!s.isAgentDead());
	
	MoveAction::cptr action = s.getApplicableActions().at(a);
	
	// We generate here the new state after applying the given action. For that, we compute each of the new state attributes.
	// 1. The new position of the agent
	Engine::Point2D<int> position = action->getResultingPosition(s.getAgentPosition());
	
	// 2. The precomputed list of valid actions
	auto validActions = MoveAction::computeApplicableActions(getWorld(), position);

	// 3. The new resource raster, that will be affected by the agent's consumption and by the natural growth of resources
	// The branch MDPRaster has an alternative here to try to reduce the performance impact of this copy
	Engine::DynamicRaster resourceRaster(s.getResourceRaster()); // copy the map of resources from the previous state
	
	// 4. The new amount of resources held by the agent is obtained by applying the logic of resource consumption on
	//    the resources that the agent already had, plus the ones that she collected.
	int collectedResources = ModelAgent::collectResources(resourceRaster, position); // (This already updates the raster)
	int resources = ModelAgent::consumeDailyResources(s.getAgentResources() + collectedResources);
	
	// 5. Only after subtracting the agent's consumption, we apply the natural growth to the raster.
	resourceRaster.updateRasterIncrement();
	
	
	// 6. Check for the reproduction of the agent - Note that we do not actually reproduce the agent,
	// only reduce the amount of resources, but this does not affect the cost, as we do not want to 
	// induce the agent not to reproduce.
    if (ModelAgent::checkReproduction(resources)) {
		resources = ModelAgent::consumeResourcesOnReproduction(resources);
	}	
	

	// TODO: Using std::move for the resource raster, after implementing a move constructor, would increase performance.
	outcomes.push_back(std::make_pair(
		MDPState(position, resourceRaster, resources, validActions), // The next problem state
		1.0 // The probability with which this state is reached when applying action a on state s
	));
}

const Engine::World& MDPProblem::getWorld() const {
	return *_agent.getWorld();
}


  
} // namespaces
