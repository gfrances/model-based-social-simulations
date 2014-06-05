#ifndef __MDP_CONTROLLER_HXX__
#define __MDP_CONTROLLER_HXX__

#include "AgentController.hxx"

namespace Model
{

/**
 * An MDP controller models the surrounding world as a Markov Decision Process, runs UCT
 * for a fixed number of iterations and planning horizon on it,
 * and chooses the action deemed to be optimal.
 */
class MDPController : public AgentController
{
public:
	MDPController() {}
	virtual ~MDPController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		// TODO
		return (Engine::Action*) 0;
	}
};

}

#endif
