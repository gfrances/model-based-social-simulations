
#ifndef __ModelAgent_hxx__
#define __ModelAgent_hxx__

#include <Agent.hxx>
#include <controllers/AgentController.hxx>
#include <string>
#include <memory>


namespace Model
{

class ModelAgent : public Engine::Agent
{
private:
	const AgentController::cptr _controller;
	int _resources; // MpiBasicAttribute
	int _numChildren; // MpiBasicAttribute

	Engine::Action* getLastSelectedAction();
	
	//! Helper to return the agent's world's resource raster, non-const version
	Engine::DynamicRaster& getResourceRaster();
	
	
	//! Reproduce the agent into two agents.
	void reproduceAgent();
	
public:
	static const int INITIAL_RESOURCES = 5;
	static const int REPRODUCTION_THRESHOLD = 50;
	
	ModelAgent(unsigned id, std::shared_ptr<AgentController> controller);
	ModelAgent(const std::string& id, std::shared_ptr<AgentController> controller);
	
	virtual ~ModelAgent();

    // decision making process occurs inside this method
	void selectActions();
	
    // update state after selecting and executing actions
	void updateState();
	
	//! The resource-collection logic for an agent. Returns an amount of resources drawn uniformly at random between 1 and
	//! the total amount of resources in the given cell, and updates the raster to substract that stochastic amount.
	static int collectResources(Engine::DynamicRaster& resourceRaster, const Engine::Point2D<int>& position);
	
	//! The resource-consumption at every time step logic for an agent.
	//! Currently diminishes by one the amount of resources available to the agent.
	static int consumeDailyResources(int resources) { return resources - 1; }
	
	//! The resource-consumption after the reproduction of the agent.
	//! Currently sets the number of resources to the initial, fixed value.
	static int consumeResourcesOnReproduction(int resources) { return INITIAL_RESOURCES; }
	
	//! The death-by-starvation logic for an agent.
	//! Currently an agent dies at the end of a time step if she is holding a negative amount of resources.
	static bool checkDeathByStarvation(int resources) { return resources < 0; }
	
	//! The natural reproduction logic for an agent.
	//! Currently an agent reproduces at the end of a time step if her amount of resource reaches a certain reproduction threshold.
	static bool checkReproduction(int resources) { return resources >= REPRODUCTION_THRESHOLD; }
	
	void registerAttributes();
	void serialize();

	void setResources( int resources ) {_resources = resources; }
	int getResources() const { return _resources; }

	////////////////////////////////////////////////
	// This code has been automatically generated //
	/////// Please do not modify it ////////////////
	////////////////////////////////////////////////
	ModelAgent( void * );
	void * fillPackage();
	void sendVectorAttributes(int);
	void receiveVectorAttributes(int);
	////////////////////////////////////////////////
	//////// End of generated code /////////////////
	////////////////////////////////////////////////
	
	
	//! Helper to return the agent's world's resource raster.
	const Engine::DynamicRaster& getResourceRaster() const;
};

} // namespace Model

#endif // __ModelAgent_hxx__

