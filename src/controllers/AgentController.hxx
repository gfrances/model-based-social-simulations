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
	typedef std::shared_ptr<AgentController> ptr;
	typedef std::shared_ptr<const AgentController> cptr;
	
	AgentController() {}
	virtual ~AgentController() {};
	
	//! By default, controllers are stateless and do not change upon reproduction
	//! This method can be overriden to change that behaviour.
	virtual bool doesReproduce() const { return false; }
	
	//! By default, there is no reproduction. Override this method if that is not the case.
	virtual AgentController::cptr reproduce() const { return nullptr; }
	
	
	virtual std::string getType() const = 0;
	
	//! Select the action to be performed by the agent according to the particular logic of the controller.
	virtual Engine::Action* selectAction(const ModelAgent& agent) const = 0;
	
	//! Prints a representation of the object to the given stream. By default, we simply print the type of agent.
	friend std::ostream& operator<<( std::ostream &os, const AgentController&  controller) { return controller.print(os); }
	virtual std::ostream& print(std::ostream& os) const { 
		os << getType();
		return os;
	}
};

}

#endif
