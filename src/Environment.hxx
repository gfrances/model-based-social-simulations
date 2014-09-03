
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
	
	//! Return the current world configuration.
	const EnvironmentConfig& getModelConfig() const;

public:
	static const unsigned RESOURCE_RASTER_IDX = 0;
	
	Environment(EnvironmentConfig* config, Engine::Scheduler* scheduler = 0);
	
	virtual ~Environment();
	
	ModelAgent* createAgent(const std::string id, const Engine::Point2D<int>& position);
};

} // namespace Model 

#endif // __Environment_hxx__

