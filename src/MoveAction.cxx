
#include <MoveAction.hxx>



namespace Model 
{
	// Map between the nine possible directions we consider and their corresponding coordinate pair.
	// Note that the order here matters and must correlate the order in which the directions are declared.
	const std::vector<std::pair<int, int>> MoveAction::coordinates = {
		{0, 0},
		{0, 1},
		{1, 1},
		{1, 0},
		{1, -1},
		{0, -1},
		{-1, -1},
		{-1, 0},
		{-1, 1}
	};
	
	const std::vector<std::string> MoveAction::directionStrings = {
		"null",
		"north",
		"north-east",
		"east",
		"south-east",
		"south",
		"south-west",
		"west",
		"north-west"
	};

} // namespace Model
