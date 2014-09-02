

#ifndef __CONTROLLERS_MDP_STATE_HXX__
#define __CONTROLLERS_MDP_STATE_HXX__

#include <HashTable.hxx>
#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <Environment.hxx>
#include <MoveAction.hxx>

namespace Model
{

/**
 * An state of the Markov Decision Process, encoding the necessary information about the world and the context of
 * the agent in a given time step. 
 */
class MDPState
{
protected:
	//! The location of the agent in the current state.
	Engine::Point2D<int> _agentPosition;
	
	//! The map of resources
	Engine::DynamicRaster _resourceRaster;
	
	//! The amount of resources that the agent holds.
	int _agentResources;
	
	//! We cache here pointers to the actions that are applicable in the current state.
	std::vector<MoveAction::cptr> _applicableActions;
	
	//! The hash value of the state
	unsigned _hash;
	
	//! Generate the hash value of the state from the relevant state information
	unsigned generateHash() {
		Engine::HashKey hashKey;
		hashKey.add(_agentPosition._x);
		hashKey.add(_agentPosition._y);
		return hashKey.code();
	}
	
	//! Compute and cache the hash key of the state.
	void storeHash() { _hash = generateHash(); }
	
public:
	//!
	// TODO - Implement a move constructor to optimize performance
	MDPState(const Engine::Point2D<int>& position,
			 const Engine::DynamicRaster& resourceRaster,
		     int agentResources,
		     const std::vector<MoveAction::cptr>& applicableActions) :
		_agentPosition(position),
		_resourceRaster(resourceRaster),
		_agentResources(agentResources),
		_applicableActions(applicableActions),
		_hash(generateHash())
	{}
	virtual ~MDPState() {}
	
	//! Copy constructor
	MDPState(const MDPState& other) :
		_agentPosition(other._agentPosition),
		_resourceRaster(other._resourceRaster),
		_agentResources(other._agentResources),
		_applicableActions(other._applicableActions),
		_hash( other._hash ) {
		
	}
	
	//! Assignment operator
	MDPState& operator=(const MDPState& other) {
		if (this != &other) {
			_agentPosition = other._agentPosition;
			_resourceRaster = other._resourceRaster;
			_agentResources = other._agentResources;
			_applicableActions = other._applicableActions; // Note that we do not need to deep copy.
			_hash = other._hash;
        }
        return *this;
	}
	
	//! Equality operator
	//! Note that there is no need to compare the available actions, since they are deterministically derived
	//! from the rest of the state attributes.
	bool operator==(const MDPState& other) const {
		if (hash() != other.hash()) return false;
		
		if (_agentResources != other._agentResources) return false; // TODO - Compare a discretized amount of resources?
		
		if (_agentPosition != other._agentPosition) return false;
		
		// TODO - Compare the _discretized_, and possibly with limited information, resource map.
		if (_resourceRaster != other._resourceRaster) return false;
		
		return true;
	}
	
	bool operator!=(const MDPState& other) const { return !(*this == other); }
	
	//! Return the position of the agent in the current state.
	const Engine::Point2D<int>& getAgentPosition() const { return _agentPosition; }
	
	//! Return the amount of resources held by the agent in the current state.
	int getAgentResources() const { return _agentResources; }
	
	//! The agent is marked as dead following the death-by-starvation logic.
	int isAgentDead() const { return ModelAgent::checkDeathByStarvation(_agentResources); }
	
	//! Return a vector with all the actions that are applicable in the current state.
	const std::vector<MoveAction::cptr> getApplicableActions() const { return _applicableActions; }
	
	//! Return the resource raster of the state
	const Engine::DynamicRaster& getResourceRaster() const { return _resourceRaster; }
	
	//! Return the state hash code. Note that this method is required by the mdp-engine library.
	unsigned hash() const { return _hash;  }
};

} // namespaces

#endif
