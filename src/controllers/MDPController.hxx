#ifndef __MDP_CONTROLLER_HXX__
#define __MDP_CONTROLLER_HXX__

#include "AgentController.hxx"
#include <EnvironmentConfig.hxx>
#include "mdp/MDPState.hxx"
#include "mdp/MDPProblem.hxx"
#include <MoveAction.hxx>
#include <engine/policy.h>
#include <engine/uct.h>

namespace Model
{

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
	MDPController(const ControllerConfig& config) 
		: _config(config)
		{}
	virtual ~MDPController() {}
	
	//! In order to select the appropriate action, we setup an MDPProblem and "solve it" using UCT.
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		// The object with all the MDP information and methods.
		// Note that we need to store this object here, since the policy object object keeps only a const reference.
		MDPProblem problem(_config, agent);
		
		// The policy that we will use as the UCT base policy.
		// For now we are using a random base policy, but other more sophisticated options could also be used.
		// Note that we need to store this object here, since the policy object object keeps only a const reference.		
		RandomPolicy basePolicy(problem);
		
		// The UCT policy object that we will use to make the decisions.
		UCTPolicy uctPolicy(basePolicy, _config.getWidth(), _config.getHorizon(), _config.getExplorationBonus(), false );
		
		Problem::action_t actionIdx = uctPolicy(problem.init());
		MoveAction::cptr action = problem.init().getApplicableActions().at(actionIdx);
		return new MoveAction(*action); // The ownership of the action will belong to the Engine::Agent class.
	}
};

} // namespaces

#endif
