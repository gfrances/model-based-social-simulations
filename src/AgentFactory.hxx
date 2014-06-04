
#ifndef __AGENT_FACTORY_HXX__
#define __AGENT_FACTORY_HXX__

#include <EnvironmentConfig.hxx>
#include <AgentController.hxx>
#include <ModelAgent.hxx>
#include <MDPController.hxx>
#include <LazyController.hxx>
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
	
	ModelAgent* createAgent(unsigned id) {
		auto controller = createController();
		ModelAgent* agent = new ModelAgent(id, controller);
		return agent;
	}
	
	std::shared_ptr<AgentController> createController() const {
		if (_config.controllerType == AgentControllerType::MDP) {
			return std::make_shared<MDPController>();
// 		} else if (_config.controllerType == AgentControllerType::random) {
// 			return std::make_shared<LazyController>();
		} else if (_config.controllerType == AgentControllerType::lazy) {
			return std::make_shared<LazyController>();
		}
		assert(false); // Should never get here
	}
};

} // namespace Model 

#endif

