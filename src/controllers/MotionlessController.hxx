#ifndef __MOTIONLESS_CONTROLLER_HXX__
#define __MOTIONLESS_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"

namespace Model
{

/**
 * A simple controller that always decides NOT to move.
 */
class MotionlessController : public AgentController
{
public:
	MotionlessController() {}
	virtual ~MotionlessController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		return new MoveAction(MoveAction::DIRECTION_STAY);
	}
	
	virtual std::string getType() const { return "motionless"; }
};

}

#endif
