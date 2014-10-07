
#ifndef __ModelAgent_hxx__
#define __ModelAgent_hxx__

#include <Agent.hxx>
#include <controllers/AgentController.hxx>
#include "EnvironmentConfig.hxx"
#include <string>
#include <memory>


namespace Model
{

class MDPRaster; class Environment;
	
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
	
	//! Helper to retrieve the model configuration
	inline const EnvironmentConfig& getConfiguration() const { 
		return static_cast<const EnvironmentConfig&>(getWorld()->getConfig());
	}
	
	
public:
	
	ModelAgent(unsigned id, Environment* world, const AgentController::cptr controller);
	ModelAgent(const std::string& id, Environment* world, const AgentController::cptr controller);
	
	virtual ~ModelAgent();
	
	//! We overload the type detection routine to make use of the controller type
	virtual std::string getType() const;

    // The decision-making process occurs inside this method
	void selectActions();
	
    // Update the agent's state after the selection and execution of actions
	void updateState();
	
	//! The resource-collection logic for an agent. Returns an amount of resources drawn uniformly at random between 1 and
	//! the total amount of resources in the given cell, and updates the raster to substract that stochastic amount.
	//! Note that we have an specific method for MDPRasters, which proceed a bit differently, for more clarity.
	static int collectResources(Engine::DynamicRaster& resourceRaster, const Engine::Point2D<int>& position);
	static int collectResources(MDPRaster& resourceRaster, const Engine::Point2D<int>& position);
	
	//! The resource-consumption at every time step logic for an agent.
	//! Currently diminishes by one the amount of resources available to the agent.
	int dailyResourceConsumption() const { return getConfiguration().getConsumptionFactor(); }
	
	//! The amount of resources with which a new agent is born
	int initialResources() const { return dailyResourceConsumption() * 5; }
	
	//! The resource threshold that triggers the reproduction of the agent into two different agents.
	int reproductionThreshold() const { return dailyResourceConsumption() * 20; }
	
	//! The resource-consumption after the reproduction of the agent.
	//! Currently sets the number of resources to the initial, fixed value.
	int consumeResourcesOnReproduction(int resources) const { return initialResources(); }
	
	//! The death-by-starvation logic for an agent.
	//! Currently an agent dies at the end of a time step if she is holding a negative amount of resources.
	static bool checkDeathByStarvation(int resources) { return resources < 0; }
	
	//! The natural reproduction logic for an agent.
	//! Currently an agent reproduces at the end of a time step if her amount of resource reaches a certain reproduction threshold.
	bool checkReproduction(int resources) const { 
		if (!getConfiguration().doAgentsReproduce()) return false;
		return resources >= reproductionThreshold();
	}
	
	//! Accessors for the resources attribute
	void setResources( int resources ) {_resources = resources; }
	int getResources() const { return _resources; }
	
	//! Accessor for the controller
	const AgentController::cptr getController() const { return _controller; }
	
	//! Prints a representation of the state to the given stream. Redefined from the parent class.
	virtual std::ostream& print(std::ostream& os) const;
	
	//! Helper to return the agent's world's resource raster.
	const Engine::DynamicRaster& getResourceRaster() const;
	
	void registerAttributes();
	void serialize();

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
};

} // namespace Model

#endif // __ModelAgent_hxx__

