#ifndef __LAZY_CONTROLLER_HXX__
#define __LAZY_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"
#include <ModelAgent.hxx>
#include <EnvironmentConfig.hxx>

namespace Model
{

/**
 * The lazy controller only moves when there are not enough resources in the current cell,
 * and picks up arbitrarily any surrounding cell with enough resources, or any other arbitrary
 * cell if that is not possible.
 * The operation of assessing whether there are enough resources is affected by an `alpha` parameter
 * that ranges from 0 to 1: If the current amount of resources multiplied by alpha is at least equal
 * to the agent's needs, then she considers that the amount is sufficient to stay.
 */
class LazyController : public AgentController
{
protected:
	//!
	float _alpha;

public:
	LazyController(const ControllerConfig& config) : _alpha(config.alpha) {
		assert(_alpha > 0 && _alpha <= 1);
	}
	virtual ~LazyController() {}
	
	//! Returns true iff the given position satisfies the agent according to the logic of the controller.
	bool positionSatisfiesAgent(const ModelAgent& agent, const Engine::Point2D<int>& position) const {
		return _alpha * agent.getResourceRaster().getValue(position) >= agent.dailyResourceConsumption();
	}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {

		const Engine::Point2D<int>& current = agent.getPosition();
		const Engine::World* world = agent.getWorld();
		assert(world);
		
		// If there are enough resources for the agent daily needs in the current cell, the agent stays.
		if (positionSatisfiesAgent(agent, current)) {
			return new MoveAction(0);
		}
		
		// Otherwise, we choose an arbitrary cell with enough resources for the daily needs.
		
		// We shuffle all the possible directions
		std::vector<unsigned> indexes(MoveAction::DIRECTIONS.size() - 1);
		std::iota(indexes.begin(), indexes.end(), 1); // Start at one, since we don't need to consider the current location.
		std::random_shuffle(indexes.begin(), indexes.end());
		
		// And consider them one by one, stopping after the first which satisfies the agent's needs.
		for (unsigned idx:indexes) {
			const Engine::Point2D<int> point = current + MoveAction::DIRECTIONS[idx];
			if (world->checkPosition(point) && positionSatisfiesAgent(agent, point)) {
				return new MoveAction(idx);
			}
		}
		
		// If no valid cell had enough resources, we go to a randomly selected cell.
		return MoveAction::createRandomAction(agent);
	}
	
	virtual std::string getType() const { return "lazy"; }
};

}

#endif
