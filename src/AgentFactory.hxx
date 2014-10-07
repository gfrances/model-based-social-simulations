
#ifndef __AGENT_FACTORY_HXX__
#define __AGENT_FACTORY_HXX__


#include <map>
#include <controllers/AgentController.hxx>

namespace Model 
{

class Environment; class ModelAgent; class ControllerConfig;

class AgentFactory
{

protected:
	//! Maps controller names (which act as IDs) to controller objects.
	//! We use one controller object for all the agents of that type.
	std::map<std::string, AgentController::cptr> controllers;
	
	
public:
	AgentFactory();
	~AgentFactory() {}
	
	void registerControllerType(const ControllerConfig& config);
	
	
	//! Simply create a controller of the type specified in the configuration and inject it to the agent.
	ModelAgent* createAgent(unsigned id, Environment* world, const std::string& type) const;
	ModelAgent* createAgent(const std::string id, Environment* world, const std::string& type) const;
	
	
protected:
	
	//! Creates a controller of the given type with the given config.
	void createController(const ControllerConfig& config);
	
	//! Retrieve from the controller pool or create a controller of the given type.
	AgentController::cptr getController(const std::string& type) const;
};

} // namespace Model 

#endif

