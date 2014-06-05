#ifndef __MOVE_ACTION_HXX__
#define __MOVE_ACTION_HXX__

#include <Action.hxx>
#include <Logger.hxx>

namespace Engine { class Agent; }

namespace Model
{
/**
  * We currently only consider one type of action, a MoveAction, which is always associated with one direction.
  * Directions are represented by a pair of integers (x,y), where both x and y are restricted to be {-1, 0 +1}.
  * This allows for diagonal moves as well as for "null" moves, where the direction is (0,0) and the agent stays in the same cell.
  */
class MoveAction : public Engine::Action
{
public:
	enum class Direction { ZERO = 0, N, NE, E, SE, S, SW, W, NW};
	static const unsigned NUM_DIRECTIONS = 9; // Keep in sync with the enum above! 
	static const std::vector<std::pair<int, int>> coordinates;
	static const std::vector<std::string> directionStrings;
	
protected:
	Direction _direction;
	
public:
	MoveAction(Direction direction) : _direction(direction) {}
	virtual ~MoveAction() {}

	virtual void execute(Engine::Agent& agent) {
		log_DEBUG("actions", "Executing move action: " + describe());
		// TODO - Implement the action effect.
	}
	
	virtual std::string describe() const {
		return "move (" + directionStrings[(int)_direction] + ")";
	}
	
	/**
	 * Returns one of the nine possible move actions chosen uniformly at random.
	 */
	static MoveAction* createRandomAction() {
		int intDirection = Engine::GeneralState::statistics().getUniformDistValue(0, NUM_DIRECTIONS);
		return new MoveAction(static_cast<Direction>(intDirection));
	}
};

}

#endif
