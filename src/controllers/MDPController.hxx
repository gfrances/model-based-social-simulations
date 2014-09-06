
#ifndef __MDP_CONTROLLER_HXX__
#define __MDP_CONTROLLER_HXX__

#include "AgentController.hxx"
// #include "mdp/MDPState.hxx"
#include <engine/policy.h>
#include <engine/uct.h>

namespace Model
{

class MDPState; class MDPProblem; class ControllerConfig;

/**
 * An MDP controller models the surrounding world as a Markov Decision Process, runs UCT
 * for a fixed number of iterations and planning horizon on it,
 * and chooses the action deemed to be optimal.
 */
class MDPController : public AgentController
{
protected:
	typedef Online::Policy::random_t<MDPState> RandomPolicy;
	typedef Online::Policy::UCT::uct_t<MDPState> UCTPolicy;
	
	//! The configuration object.
	const ControllerConfig& _config;
	
public:
	MDPController(const ControllerConfig& config);
	virtual ~MDPController() {}
	
	virtual std::string getType() const { return "mdp"; }
	
	//! In order to select the appropriate action, we setup an MDPProblem and "solve it" using UCT.
	virtual Engine::Action* selectAction(const ModelAgent& agent) const;
};

} // namespaces

#endif
