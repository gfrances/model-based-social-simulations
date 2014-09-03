
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>
#include <boost/lexical_cast.hpp>


namespace Model
{

EnvironmentConfig::EnvironmentConfig() : _numAgents(0), _size(0, 0), _fileName("none"), _logdir("")
{
}

EnvironmentConfig::~EnvironmentConfig()
{
}

void EnvironmentConfig::extractParticularAttribs(TiXmlElement * root)
{
	// We retrieve the log dir again... no better way of doing it without modifying the base classes.
	retrieveAttributeMandatory( root->FirstChildElement("output"), "logsDir", _logdir); 
	
	TiXmlElement * element = root->FirstChildElement("size");
	retrieveAttributeMandatory( element, "width", _size._width);
	retrieveAttributeMandatory( element, "height", _size._height);

    element = root->FirstChildElement("environment");
    retrieveAttributeMandatory(element, "fileName", _fileName);
    
	std::string data;
    element = root->FirstChildElement("agents");
    retrieveAttributeMandatory(element, "initPop", data);
	_numAgents = boost::lexical_cast<unsigned>(data);
	
	retrieveControllerConfig(element->FirstChildElement("controller"));
}
	
const Engine::Size<int> & EnvironmentConfig::getSize() const
{
	return _size;
}

void EnvironmentConfig::retrieveControllerConfig(TiXmlElement* controller) {
	
	// The controller type
	std::string data;
    retrieveAttributeMandatory(controller, "type", data);
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
	
	retrieveAttributeMandatory(controller, "horizon", data);
	controllerConfig.horizon = boost::lexical_cast<unsigned>(data);
	
	retrieveAttributeMandatory(controller, "width", data);
	controllerConfig.width = boost::lexical_cast<unsigned>(data);
	
	retrieveAttributeMandatory(controller, "explorationBonus", data);
	controllerConfig.explorationBonus = boost::lexical_cast<float>(data);
}

} // namespace Model

