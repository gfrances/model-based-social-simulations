#ifndef __AGENT_CONTROLLER__
#define __AGENT_CONTROLLER__

#include <memory>

namespace Engine { class Action; }  // forward declaration

namespace Model
{

class ModelAgent;

/**
 * An agent controller is in charge of selecting an action for a given agent following some particular
 * criteria to be defined in the subclasses.
 */
class AgentController
{
public:
	typedef std::shared_ptr<const AgentController> cptr;
	
	AgentController() {}
	virtual ~AgentController() {};
	
	virtual std::string getType() const = 0;
	
	//! Select the action to be performed by the agent according to the particular logic of the controller.
	virtual Engine::Action* selectAction(const ModelAgent& agent) const = 0;
};

}

#endif
