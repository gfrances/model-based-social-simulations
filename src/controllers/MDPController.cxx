
#include "MDPController.hxx"
#include "mdp/MDPProblem.hxx"
#include <EnvironmentConfig.hxx>
#include <MoveAction.hxx>

namespace Model
{
	

MDPController::MDPController(const ControllerConfig& config) 
	: _config(config)
	{}


Engine::Action* MDPController::selectAction(const ModelAgent& agent) const {
	// The object with all the MDP information and methods.
	// Note that we need to store this object here, since the policy object object keeps only a const reference.
	MDPProblem problem(_config, agent);
	
	// The policy that we will use as the UCT base policy.
	// For now we are using a random base policy, but other more sophisticated options could also be used.
	// Note that we need to store this object here, since the policy object object keeps only a const reference.		
	RandomPolicy basePolicy(problem);
	
	// The UCT policy object that we will use to make the decisions.
	UCTPolicy uctPolicy(basePolicy, _config.getWidth(), _config.getHorizon(), _config.getExplorationBonus(), true );
	
	Problem::action_t actionIdx = uctPolicy(problem.init());
	MoveAction::cptr action = problem.init().getApplicableActions().at(actionIdx);
	return new MoveAction(*action); // The ownership of the action will belong to the Engine::Agent class.
}

} // namespaces

