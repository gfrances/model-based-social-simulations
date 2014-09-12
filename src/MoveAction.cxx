
#include <MoveAction.hxx>



namespace Model 
{
	// Map between the nine possible directions we consider and their corresponding coordinate pair.
	// Note that the order here matters and must correlate the order in which the directions are declared.
	const std::vector<Engine::Point2D<int>> MoveAction::DIRECTIONS = {
		Engine::Point2D<int>(0, 0),   // Null move, i.e. stay in the same cell.
		Engine::Point2D<int>(0, 1),   // move-N
		Engine::Point2D<int>(1, 1),   // move-NE
		Engine::Point2D<int>(1, 0),   // move-E
		Engine::Point2D<int>(1, -1),  // move-SE
		Engine::Point2D<int>(0, -1),  // move-S
		Engine::Point2D<int>(-1, -1), // move-SW
		Engine::Point2D<int>(-1, 0),  // move-W
		Engine::Point2D<int>(-1, 1)   // move-NW
	};
	
	const MoveAction::DirectionIdx MoveAction::DIRECTION_STAY = 0;
	
	const std::vector<std::string> MoveAction::names = {
		"stay",
		"move-north",
		"move-north-east",
		"move-east",
		"move-south-east",
		"move-south",
		"move-south-west",
		"move-west",
		"move-north-west"
	};

} // namespace Model
