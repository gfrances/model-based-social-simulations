

#include <AgentFactory.hxx>
#include <ModelAgent.hxx>
#include <EnvironmentConfig.hxx>
#include <controllers/AgentController.hxx>
#include <controllers/MDPController.hxx>
#include <controllers/LazyController.hxx>
#include "controllers/MotionlessController.hxx"
#include "controllers/GreedyController.hxx"
#include "controllers/RuleBasedController.hxx"
#include "controllers/RandomController.hxx"
#include <Environment.hxx>
#include <memory>

namespace Model 
{

AgentFactory::AgentFactory() : controllers() {}
	
void AgentFactory::registerControllerType(const ControllerConfig& config) {
	createController(config);
}
	
	
ModelAgent* AgentFactory::createAgent(unsigned id, Environment* world, const std::string& type) const { return new ModelAgent(id, world, getController(type)); }
ModelAgent* AgentFactory::createAgent(const std::string id, Environment* world, const std::string& type) const { return new ModelAgent(id, world, getController(type)); }
	
	
void AgentFactory::createController(const ControllerConfig& config) {
	std::shared_ptr<AgentController> controller;
	const std::string& type = config.getType();
	
 	if (type == "MDP") {
		controller = std::make_shared<MDPController>(config);
 	} else if (type == "random") {
		controller = std::make_shared<RandomController>();
 	} else if (type == "motionless") {
		controller = std::make_shared<MotionlessController>();
 	} else if (type == "lazy") {
		controller =  std::make_shared<LazyController>(config);
 	} else if (type == "greedy") {
		controller = std::make_shared<GreedyController>();
 	} else if (type == "rule") {
		controller = std::make_shared<RuleBasedController>();
 	} else {
 		throw Engine::Exception("Unknown controller type"); // Should never get here!
 	}
	
	controllers.insert(std::make_pair(type, controller));
}


AgentController::cptr AgentFactory::getController(const std::string& type) const {
	auto controllerIt = controllers.find(type);
	if (controllerIt != controllers.end()) {
		return controllerIt->second;
	}
	throw Engine::Exception("Tried to instantiate a controller type previously undeclared");
}


} // namespace Model 


