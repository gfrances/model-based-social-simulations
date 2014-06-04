#ifndef __MDP_CONTROLLER_HXX__
#define __MDP_CONTROLLER_HXX__

#include "AgentController.hxx"

namespace Engine { class Action; }  // forward declaration

namespace Model
{

class ModelAgent;

class MDPController : public AgentController
{
public:
	MDPController() {}
	virtual ~MDPController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) {
		// TODO
		return (Engine::Action*) 0;
	}
};

}

#endif
