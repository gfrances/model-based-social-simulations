
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
class ModelAgent;

class Environment : public Engine::World
{
	void createRasters();
	void createAgents();

	const EnvironmentConfig& _config;
public:
	static const unsigned RESOURCE_RASTER_IDX = 0;
	
	Environment(const EnvironmentConfig & config, Engine::Simulation & simulation, Engine::Scheduler * scheduler = 0);
	virtual ~Environment();
	
	ModelAgent* createAgent(const std::string id, const Engine::Point2D<int>& position);
};

} // namespace Model 

#endif // __Environment_hxx__

