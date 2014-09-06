#ifndef __RANDOM_CONTROLLER_HXX__
#define __RANDOM_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include <MoveAction.hxx>

namespace Model
{

/**
 * A controller that always chooses one of the nine move actions
 * that are valid for the given agent uniformly at random.
 */
class RandomController : public AgentController
{
public:
	RandomController() {}
	virtual ~RandomController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		return MoveAction::createRandomAction(agent);
	}
	
	virtual std::string getType() const { return "random"; }
};

}

#endif
