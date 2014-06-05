#ifndef __RANDOM_CONTROLLER_HXX__
#define __RANDOM_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"

namespace Model
{

/**
 * A controller that always chooses one of the nine possible move actions uniformly at random.
 */
class RandomController : public AgentController
{
public:
	MotionlessController() {}
	virtual ~MotionlessController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		return MoveAction::createRandomAction();
	}
};

}

#endif
