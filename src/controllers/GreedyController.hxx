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
		unsigned best_idx = 0,
				 num_directions = MoveAction::DIRECTIONS.size();
		int max = -1;
		
		// Shuffle the possible directions to introduce more diversity
		std::vector<unsigned> indexes(num_directions);
		std::iota(std::begin(indexes), std::end(indexes), 0); // fill the index vector with the range [0..num_directions-1]
		std::random_shuffle(indexes.begin(), indexes.end());
		
		for (unsigned idx:indexes) {
			const Engine::Point2D<int> point = current + MoveAction::DIRECTIONS[idx];
			if (world->checkPosition(point)) {
				int res = raster.getValue(point);
				if (res > max) {
					max = res;
					best_idx = idx;
				}
			}
		}
		return new MoveAction(best_idx);
	}
	
	virtual std::string getType() const { return "greedy"; }
};

}

#endif
