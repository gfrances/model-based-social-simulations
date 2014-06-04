#ifndef __AGENT_CONTROLLER__
#define __AGENT_CONTROLLER__

#include <memory>

namespace Engine { class Action; }  // forward declaration

namespace Model
{

class ModelAgent;

class AgentController
{
public:
	typedef std::shared_ptr<const AgentController> cptr;
	
	AgentController() {}
	virtual ~AgentController() {};
	
	//! Select the action to be performed by the agent according to the particular logic of the controller.
	virtual Engine::Action* selectAction(const ModelAgent& agent) = 0;
};

}

#endif
