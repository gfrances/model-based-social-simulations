
#ifndef __CONTROLLERS_MDP_MDPPROBLEM_HXX__
#define __CONTROLLERS_MDP_MDPPROBLEM_HXX__

#include <EnvironmentConfig.hxx>
#include "MDPState.hxx"
#include <ModelAgent.hxx>
#include <EnvironmentConfig.hxx>
#include <engine/problem.h>
#include <vector>
#include <map>

namespace Model
{

/**
 * An MDPProblem (called 'MDPModel' in elsewhere, but MDPProblem here for consistency with the inheritance chain)
 * contains all the information relevant to the Markov Decision Process, most notably the procedures by which:
 * 
 * (1) An MDP state is generated from the situation of a given agent in the world (@see MDPProblem::setup),
 * (2) The applicability of an action in a given MDP state is assessed (@see MDPProblem::applicable),
 * (3) A new MDP state is generated from the application of an action to a preivous MDP state (@see MDPProblem::next),
 * (4) The cost of applying an action in a given MDP state is assessed (@see MDPProblem::cost).
 */
class MDPProblem : public Problem::problem_t<MDPState>
{
protected:
	typedef std::vector<std::pair<MDPState, float>> OutcomeVector;

	//! The configuration parameters of the controller
	const ControllerConfig& _config;
	
	//! The agent that models the world as an MDP problem.
	const ModelAgent& _agent;
	
	//! The initial state of the MDP problem
	const MDPState initialState;

public:

	//! Create the MDP problem with the particular information obtained from the situation of the agent in the current moment.
	//! This basically boils down to generating the MDPState describing that situation.
	MDPProblem(const ControllerConfig& config, const ModelAgent& agent);
	virtual ~MDPProblem();
	
	//! Return the world in which the MDP takes place.
	const Engine::World& getWorld() const;
	
	/******************************************************
	 * Interface inherited from Problem::problem_t<T>
	 ******************************************************/
	virtual Problem::action_t number_actions( const MDPState &s ) const;
	
	virtual const MDPState& init() const { return initialState; };
	
	//! We have no terminal states.
	virtual bool terminal( const MDPState &s ) const { return false; };
	
	//! The only dead ends occur when the agent is dead
	virtual bool dead_end( const MDPState &s ) const { return s.isAgentDead(); }
	
	virtual bool applicable( const MDPState &s, Problem::action_t a ) const;
	
	//! The cost of a (move) action is inversely proportional to the amount of resources of the destination cell.
	virtual float cost( const MDPState &s, Problem::action_t a ) const;
	
	//! Construct the state resulting from the application of `action` to the previous state `s`
	//! and leaves it in the outcome vector.
	virtual void next( const MDPState &s, Problem::action_t a, OutcomeVector& outcomes ) const;

	virtual	void print( std::ostream& os ) const {}
};

} // namespaces

#endif
