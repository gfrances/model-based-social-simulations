#ifndef __GREEDY_CONTROLLER_HXX__
#define __GREEDY_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"

namespace Model
{

/**
 * The greedy controller always moves to the cell with the highest amount of available resources.
 * This includes staying in the current cell, if that's the one with the maximum resources.
 */
class GreedyController : public AgentController
{
public:
	GreedyController() {}
	virtual ~GreedyController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		//TODO - IMPLEMENT
		assert(false);
		return MoveAction::createRandomAction(agent);
	}
	
	virtual std::string getType() const { return "Greedy"; }
};

}

#endif
