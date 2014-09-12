#ifndef __GREEDY_CONTROLLER_HXX__
#define __GREEDY_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"

namespace Model
{

/**
 * The greedy controller always moves to the cell with the highest amount of available resources.
 * This includes staying in the current cell, if that's the one with the maximum amount of resources.
 */
class GreedyController : public AgentController
{
public:
	GreedyController() {}
	virtual ~GreedyController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		
		const auto& current = agent.getPosition();
		const Engine::World* world = agent.getWorld();
		assert(world);
		const Engine::DynamicRaster& raster = agent.getResourceRaster();
		
		// We start by assuming that the best option is the no-move action
		unsigned best_idx = 0;
		int max = raster.getValue(current);
		
		// Note that we skip the first element, the "no-move" direction that we have already processed.
		for (unsigned i = 1; i < MoveAction::DIRECTIONS.size(); ++i) {
			const Engine::Point2D<int> point = current + MoveAction::DIRECTIONS[i];
			if (world->checkPosition(point)) {
				if (int res = raster.getValue(point) > max) {
					max = res;
					best_idx = i;
				}
			}
		}
		return new MoveAction(best_idx);
	}
	
	virtual std::string getType() const { return "greedy"; }
};

}

#endif
