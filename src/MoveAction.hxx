#ifndef __MOVE_ACTION_HXX__
#define __MOVE_ACTION_HXX__

#include <Action.hxx>
#include <Logger.hxx>
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
	//! Index of one of the possible directions in the `directions` vector.
	typedef unsigned DirectionIdx;
	
	//! A vector with all the possible directions, expressed as coordinate points.
	static const std::vector<Engine::Point2D<int>> directions;
	
	//! A helper vector with the direction names, to print info about them.
	static const std::vector<std::string> names;
	
	//! Some useful constants
	static const DirectionIdx DIRECTION_STAY;
	
protected:
	//! The actual direction of the move.
	DirectionIdx _direction;
	
public:
	MoveAction(DirectionIdx direction) : _direction(direction) {}
	virtual ~MoveAction() {}

	virtual void execute(Engine::Agent& agent) {
		log_DEBUG("actions", "Executing move action: " + describe());
		auto destination = agent.getPosition() + directions[_direction];
		assert(isValid(agent));
		agent.setPosition(destination);
	}
	
	virtual std::string describe() const {
		return names[_direction];
	}
	
	//! Return true iff the current action is valid for the given agent.
	bool isValid(const Engine::Agent& agent) {
		return _direction == DIRECTION_STAY  // The no-move direction is always valid.
		       || agent.getWorld()->checkPosition(agent.getPosition() + directions[_direction]);
	}
	
	//! Compute a vector with the indexes of all those directions which are valid 
	//! for the given agent to take. Will contain at least one direction, the "no-move" direction.
	static std::vector<DirectionIdx> computeValidDirectionIndexes(const Engine::Agent& agent) {
		auto world = agent.getWorld();
		const auto& current = agent.getPosition();
		
		// We skip the first element, which is the "no-move" direction and is thus always valid
		std::vector<DirectionIdx> validIndexes = {0};
		for (unsigned i = 1; i < directions.size(); ++i) {
			if (world->checkPosition(current + directions[i])) {
				validIndexes.push_back(i);
			}
		}
		return validIndexes;
	}
	
	//! Returns one of the nine possible move actions chosen uniformly at random.
// 	static MoveAction* createRandomAction() {
// 		int idx = Engine::GeneralState::statistics().getUniformDistValue(0, directions.size());
// 		return new MoveAction(idx);
// 	}
	
	//! Returns one of the actions that are valid for the given agent, chosen uniformly at random.
	static MoveAction* createRandomAction(const Engine::Agent& agent) {
		auto validIndexes = computeValidDirectionIndexes(agent);
		// Note that the vector is guaranteed to have size at least one
		int idx = Engine::GeneralState::statistics().getUniformDistValue(0, validIndexes.size()-1);
		return new MoveAction(validIndexes[idx]);
	}	
};

}

#endif
