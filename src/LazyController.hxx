#ifndef __LAZY_CONTROLLER_HXX__
#define __LAZY_CONTROLLER_HXX__

#include "AgentController.hxx"

namespace Engine { class Action; }  // forward declaration

namespace Model
{

class ModelAgent;

class LazyController : public AgentController
{
public:
	LazyController() {}
	virtual ~LazyController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) {
		// TODO
		return (Engine::Action*) 0;
	}
};

}

#endif
