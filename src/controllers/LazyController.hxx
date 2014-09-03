#ifndef __LAZY_CONTROLLER_HXX__
#define __LAZY_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"

namespace Model
{

/**
 * The lazy controller only moves when there are not enough resources in the current cell,
 * and picks up arbitrarily any surrounding cell with enough resources, or any other arbitrary
 * cell if that is not possible.
 */
class LazyController : public AgentController
{
public:
	LazyController() {}
	virtual ~LazyController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		//TODO - IMPLEMENT
		assert(false);
		return MoveAction::createRandomAction(agent);
	}
	
	virtual std::string getType() const { return "Lazy"; }
};

}

#endif
