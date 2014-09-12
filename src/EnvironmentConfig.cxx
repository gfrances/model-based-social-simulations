
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>
#include <iostream>
#include <tinyxml.h>
#include <algorithm>

namespace Model
{
	
const std::vector<std::string> EnvironmentConfig::ALLOWED_CONTROLLERS = {"MDP", "random", "motionless", "lazy", "greedy", "rule"};

EnvironmentConfig::EnvironmentConfig(const std::string& filename) : 
	Engine::Config(filename),
	_size(0, 0), map(""), _logdir(""), _controllers(0)
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
	
	// Load the configuration corresponding to the agent controllers
	loadControllerParams();
}
	
void EnvironmentConfig::loadSingleControllerConfig(TiXmlElement* element) {
	ControllerConfig config;
	config.type = getParamStrFromElem(element, "type");
	config.population = getParamUnsignedFromElem(element, "population");
	
	if (std::find(ALLOWED_CONTROLLERS.begin(), ALLOWED_CONTROLLERS.end(), config.type) == ALLOWED_CONTROLLERS.end()) {
		throw Engine::Exception("Unknown agent controller type '" + config.type + "'");
	}
	
 	if (config.type == "MDP") {
		config.horizon = getParamUnsignedFromElem(element, "horizon");
		config.width = getParamUnsignedFromElem(element, "width");
		config.explorationBonus = getParamUnsignedFromElem(element, "explorationBonus");		
 	} else if (config.type == "lazy") {
		config.alpha = getParamFloatFromElem(element, "alpha");
	}
	
 	_controllers.push_back(config);
}

void EnvironmentConfig::loadControllerParams() {
	for(TiXmlElement* controller_element = findElement("agents/controller"); controller_element; controller_element = controller_element->NextSiblingElement()) {
		loadSingleControllerConfig(controller_element);
	}
}

} // namespace Model

