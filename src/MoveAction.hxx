#ifndef __MOVE_ACTION_HXX__
#define __MOVE_ACTION_HXX__

#include <memory>

#include <Action.hxx>
#include <utils/logging.hxx>
#include <Agent.hxx>

namespace Model
{
/**
  * We currently only consider one type of action, a move action that can lead the agent to move to either
  * one of the neighbouring cells (diagonal moves are allowed) or to remain in the current cell. 
  * We thus have 9 possible directions.
  * Directions are referenced to implicitly and efficiently by their index in the `directions` vector below,
  * which actually contains the Point2D object that we need to add to any position in order to obtain
  * a new position which is the result of performing a move in that direction.
  */
class MoveAction : public Engine::Action
{
public:
	typedef std::shared_ptr<const MoveAction> cptr;
	
	//! Index of one of the possible directions in the `directions` vector.
	typedef unsigned DirectionIdx;
	
	//! A vector with all the possible directions, expressed as coordinate points.
	static const std::vector<Engine::Point2D<int>> DIRECTIONS;
	
	//! A helper vector with the direction names, to print info about them.
	static const std::vector<std::string> names;
	
	//! Some useful constants
	static const DirectionIdx DIRECTION_STAY;
	
protected:
	//! The actual direction of the move.
	DirectionIdx _direction;
	
public:
	MoveAction(DirectionIdx direction) : _direction(direction) {
		assert(_direction < DIRECTIONS.size());
	}
	virtual ~MoveAction() {}
	
	//! Copy constructor
	MoveAction(const MoveAction& other) :
	_direction(other._direction) {
		assert(_direction < DIRECTIONS.size());
	}

	//! The execution of the action simply changes the location of the agent
	//! (possibily to the same cell, if it is a no-move action
	virtual void execute(Engine::Agent& agent) {
		PDEBUG("actions", agent << " - " << *this);
		assert(isValidFor(agent));
		agent.setPosition(getResultingPosition(agent.getPosition()));
	}
	
	//! Return the new position that results from applying the current action to the given `position`
	inline Engine::Point2D<int> getResultingPosition(const Engine::Point2D<int>& position) const {
		return position + DIRECTIONS[_direction];
	}
	
	//! Return a string describing the current action unambigously.
	virtual std::string describe() const {
		return names[_direction];
	}
	
	//! Return true iff the current action is valid for the given agent.
	bool isValidFor(const Engine::Agent& agent) {
		return _direction == DIRECTION_STAY  // The no-move direction is always valid.
		       || agent.getWorld()->checkPosition(getResultingPosition(agent.getPosition()));
	}

	//! Compute a vector with all the applicable actions in the given world and for the given position.
	static std::vector<MoveAction::cptr> computeApplicableActions(const Engine::Agent& agent) {
		return computeApplicableActions(*agent.getWorld(), agent.getPosition());
	}
	
	//! Compute a vector with all the applicable actions in the given world and for the given position.
	static std::vector<MoveAction::cptr> computeApplicableActions(const Engine::World& world, const Engine::Point2D<int>& position) {
		std::vector<MoveAction::cptr> actions;
		for (unsigned idx:computeValidDirectionIndexes(world, position)) {
			actions.push_back(std::make_shared<MoveAction>(idx));
		}
		return actions;
	}	
	
	//! Compute a vector with the indexes of all those directions which are valid 
	//! for the given agent to take. Will contain at least one direction, the "no-move" direction.
	static std::vector<DirectionIdx> computeValidDirectionIndexes(const Engine::Agent& agent) {
		return computeValidDirectionIndexes(*agent.getWorld(), agent.getPosition());
	}
	
	
	//! Compute a vector with the indexes of all those directions which are valid in the given world
	//! and for the given position. Will contain at least one direction, the "no-move" direction.
	static std::vector<DirectionIdx> computeValidDirectionIndexes(const Engine::World& world, const Engine::Point2D<int>& position) {
		// We skip the first element (also in the iteration), which is the "no-move" direction and is thus always valid
		std::vector<DirectionIdx> validIndexes = {0};
		for (unsigned i = 1; i < DIRECTIONS.size(); ++i) {
			if (world.checkPosition(position + DIRECTIONS[i])) {
				validIndexes.push_back(i);
			}
		}
		return validIndexes;
	}
	
	//! Returns one of the nine possible move actions chosen uniformly at random.
	static MoveAction* createRandomAction() {
		int idx = Engine::GeneralState::statistics().getUniformDistValue(0, DIRECTIONS.size());
		return new MoveAction(idx);
 	}
	
	//! Returns one of the actions that are valid for the given agent, chosen uniformly at random.
	static MoveAction* createRandomAction(const Engine::Agent& agent) {
		auto validIndexes = computeValidDirectionIndexes(agent);
		// Note that the vector is guaranteed to have size at least one
		int idx = Engine::GeneralState::statistics().getUniformDistValue(0, validIndexes.size()-1);
		return new MoveAction(validIndexes[idx]);
	}
	
	//! Prints a representation of the state to the given stream.
	friend std::ostream& operator<<( std::ostream &os, const MoveAction& action) { return action.print(os); }
	virtual std::ostream& print(std::ostream& os) const { 
		os << describe();
		return os;
	}
};

}

#endif
