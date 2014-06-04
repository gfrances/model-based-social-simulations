
#ifndef __Environment_hxx__
#define __Environment_hxx__

#include <World.hxx>

namespace Engine
{
	class Simulation;
}

namespace Model 
{

class EnvironmentConfig;

class Environment : public Engine::World
{
	void createRasters();
	void createAgents();

	const EnvironmentConfig& _config;
public:
	Environment(const EnvironmentConfig & config, Engine::Simulation & simulation, Engine::Scheduler * scheduler = 0);
	virtual ~Environment();
};

} // namespace Model 

#endif // __Environment_hxx__

