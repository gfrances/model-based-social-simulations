
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>



namespace Model
{

EnvironmentConfig::EnvironmentConfig(const std::string& filename) : 
	Engine::Config(filename),
	_numAgents(0),  _size(0, 0), map(""), _logdir("")
{}

EnvironmentConfig::~EnvironmentConfig()
{}

void EnvironmentConfig::loadParams() {
	// We retrieve the log dir again... no better way of doing it without modifying the base classes.
	_logdir = getParamStr("output", "logsDir");

	// The map size
	_size._width = getParamInt("size", "width");
	_size._height = getParamInt("size", "height");
	
	// The map raster file
	map = getParamStr("environment", "map");
	
	// The number of agents
	_numAgents = getParamUnsigned("agents", "initPop");
	
	// Load the configuration corresponding to the agent controllers
	loadControllerParams();
}
	
const Engine::Size<int> & EnvironmentConfig::getSize() const { return _size; }

void EnvironmentConfig::loadControllerParams() {
	
	std::string data;
	
	// The controller type
	data = getParamStr("agents/controller", "type");
	
	if (data == "MDP") {
		controllerConfig.setControllerType(AgentControllerType::MDP);
	} else if (data == "random") {
		controllerConfig.setControllerType(AgentControllerType::random);
	} else if (data == "motionless") {
		controllerConfig.setControllerType(AgentControllerType::motionless);
	} else if (data == "lazy") {
		controllerConfig.setControllerType(AgentControllerType::lazy);
	} else if (data == "greedy") {
		controllerConfig.setControllerType(AgentControllerType::greedy);
	} else if (data == "rule") {
		controllerConfig.setControllerType(AgentControllerType::rule);
	} else {
		throw Engine::Exception("Unknown agent controller type '" + data + "'");
	}
	
	controllerConfig.horizon = getParamUnsigned("agents/controller", "horizon");
	controllerConfig.width = getParamUnsigned("agents/controller", "width");
	controllerConfig.explorationBonus = getParamUnsigned("agents/controller", "explorationBonus");
}

} // namespace Model

