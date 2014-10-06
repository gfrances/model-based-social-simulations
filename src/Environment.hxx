
#ifndef __Environment_hxx__
#define __Environment_hxx__

#include <World.hxx>
#include <AgentFactory.hxx>
#include <EnvironmentConfig.hxx>

namespace Engine
{
	class Simulation;
}

namespace Model 
{

class ModelAgent;

class Environment : public Engine::World
{
protected:
	void createRasters();
	void createAgents();
	
	//! Return the current world configuration.
	const EnvironmentConfig::cptr getModelConfig() const;
	
	//! The factory that creates the agents
	AgentFactory agentFactory;

public:
	static const unsigned RESOURCE_RASTER_IDX = 0;
	
	Environment(EnvironmentConfig* config, Engine::Scheduler* scheduler = 0);
	virtual ~Environment();
	
	//! Overload the parent class method to add custom logging
	void addAgent(ModelAgent* agent, bool executedAgent = true);
	void removeAgent(ModelAgent* agent);
	
	//! Overload the parent class method to add custom logging
	void step();
	
	//! Returns the units of resource that naturally grow in a map cell during `num_steps` timesteps.
	//! Currently it is one unit per timestep.
	static unsigned naturalGrowth(unsigned num_steps) { return num_steps; }
	
	void logAgentsState() const;
	void logMapState() const;	
	
};

} // namespace Model 

#endif // __Environment_hxx__

