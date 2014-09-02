
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
#include "controllers/RandomController.hxx"
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
	ModelAgent* createAgent(unsigned id) { return new ModelAgent(id, createControllerFromConfig()); }
	ModelAgent* createAgent(const std::string id) { return new ModelAgent(id, createControllerFromConfig()); }
	
	std::shared_ptr<AgentController> createControllerFromConfig() const {
		if (_config.getControllerType() == AgentControllerType::MDP) {
			return std::make_shared<MDPController>(_config);
		} else if (_config.getControllerType() == AgentControllerType::random) {
			return std::make_shared<RandomController>();
		} else if (_config.getControllerType() == AgentControllerType::motionless) {
			return std::make_shared<MotionlessController>();
		} else if (_config.getControllerType() == AgentControllerType::lazy) {
			return std::make_shared<LazyController>();
		} else if (_config.getControllerType() == AgentControllerType::greedy) {
			return std::make_shared<GreedyController>();
		} else if (_config.getControllerType() == AgentControllerType::rule) {
			return std::make_shared<RuleBasedController>();
		}
		assert(false); // Should never get here
	}
};

} // namespace Model 

#endif

