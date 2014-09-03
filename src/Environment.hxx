
#ifndef __Environment_hxx__
#define __Environment_hxx__

#include <World.hxx>
#include <AgentFactory.hxx>


namespace Engine
{
	class Simulation;
}

namespace Model 
{

class EnvironmentConfig; class ModelAgent;

class Environment : public Engine::World
{
protected:
	void createRasters();
	void createAgents();
	
	//! Return the current world configuration.
	const EnvironmentConfig& getModelConfig() const;
	
	//! The factory that creates the agents
	AgentFactory agentFactory;

public:
	static const unsigned RESOURCE_RASTER_IDX = 0;
	
	Environment(EnvironmentConfig* config, Engine::Scheduler* scheduler = 0);
	virtual ~Environment();
	
	//! Overload the parent class method to add custom logging
	void addAgent(ModelAgent* agent);
	
	//! Overload the parent class method to add custom logging
	void step();
};

} // namespace Model 

#endif // __Environment_hxx__

