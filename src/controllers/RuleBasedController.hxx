#ifndef __RULE_BASED_CONTROLLER_HXX__
#define __RULE_BASED_CONTROLLER_HXX__

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"

namespace Model
{

/**
 * A controller that implements a particular, hardcoded strategy.
 */
class RuleBasedController : public AgentController
{
public:
	RuleBasedController() {}
	virtual ~RuleBasedController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		//TODO - IMPLEMENT
		assert(false);
		return MoveAction::createRandomAction(agent);
	}
	
	virtual std::string getType() const { return "rule-based"; }
};

}

#endif
