
#ifndef __AGENT_FACTORY_HXX__
#define __AGENT_FACTORY_HXX__

#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <controllers/AgentController.hxx>
#include <controllers/MDPController.hxx>
#include <controllers/LazyController.hxx>
#include "controllers/MotionlessController.hxx"
#include "controllers/GreedyController.hxx"
#include "controllers/RuleBasedController.hxx"
#include <memory>

namespace Model 
{

class AgentFactory
{
private:
	const ControllerConfig& _config;
	
public:
	AgentFactory(const ControllerConfig& config) 
	: _config(config) {}
	
	~AgentFactory() {}
	
	//! Simply create a controller of the type specified in the configuration and inject it to the agent.
	ModelAgent* createAgent(unsigned id) {
		return new ModelAgent(id, createControllerFromConfig());
	}
	
	std::shared_ptr<AgentController> createControllerFromConfig() const {
		if (_config.controllerType == AgentControllerType::MDP) {
			return std::make_shared<MDPController>();
		} else if (_config.controllerType == AgentControllerType::random) {
			return std::make_shared<LazyController>();
		} else if (_config.controllerType == AgentControllerType::motionless) {
			return std::make_shared<MotionlessController>();
		} else if (_config.controllerType == AgentControllerType::lazy) {
			return std::make_shared<LazyController>();
		} else if (_config.controllerType == AgentControllerType::greedy) {
			return std::make_shared<GreedyController>();
		} else if (_config.controllerType == AgentControllerType::rule) {
			return std::make_shared<RuleBasedController>();
		}
		assert(false); // Should never get here
	}
};

} // namespace Model 

#endif

