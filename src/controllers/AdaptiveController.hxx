#ifndef __ADAPTIVE_CONTROLLER_HXX__
#define __ADAPTIVE_CONTROLLER_HXX__

#include <numeric>

#include <Logger.hxx>
#include "AgentController.hxx"
#include "MoveAction.hxx"

namespace Model
{

/**
 * The greedy controller always moves to the cell with the highest amount of available resources.
 * This includes staying in the current cell, if that's the one with the maximum amount of resources.
 */
class AdaptiveController : public AgentController
{
public:
	typedef std::vector<AgentController::ptr> ControllerSet;
	
protected:
	//! The different controllers among which to choose.
	const ControllerSet _controllers;
	
	//! The probability of choosing each controller when the moment of selecting a decision comes.
	const std::vector<float> _controllerProbabilities;
	
public:
	AdaptiveController(const ControllerSet& controllers, const std::vector<float>& probabilities) :
		_controllers(controllers), _controllerProbabilities(probabilities)
	{
		if (controllers.size() != probabilities.size() || std::accumulate(probabilities.begin(), probabilities.end(), 0) != 1) {
			throw std::runtime_error("Wrong probability distribution over the different strategies");
		}
	}
	virtual ~AdaptiveController() {}
	
	//! Adaptive agents mutate upon reproduction.
	virtual bool doesReproduce() const { return true; }
	virtual AgentController::cptr reproduce() const { 
		// Mutate the probabilities.
		std::vector<float> probabilities = _controllerProbabilities;
		probabilities[getUniformRandomIndex()] = Engine::GeneralState::statistics().getUniformDistValue();
		normalize(probabilities);
		return std::make_shared<AdaptiveController>(_controllers, probabilities);
	}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		// We randomly choose which controller will select the action and then delegate the decision to it.
		return _controllers[getRandomIndex()]->selectAction(agent);
	}
	
	virtual std::string getType() const { return "adaptive"; }
	
	virtual std::ostream& print(std::ostream& os) const { 
		os << getType() << "(";
		for(unsigned i = 0; i < _controllerProbabilities.size(); ++i) {
			os << *_controllers[i] << ":" << _controllerProbabilities[i] << ',';
		}
		os << ")";
		return os;
	}
protected:
	//! Returns the index of one of the controllers according to the probability distribution between controllers.
	unsigned getRandomIndex() const {
		float dice = Engine::GeneralState::statistics().getUniformDistValue();
		float accum = 0.0;
		unsigned i = 0;
		for(; i < _controllerProbabilities.size(); ++i) {
			accum += _controllerProbabilities[i];
			if (dice < accum) break;
		}
		return i;
	}
	
	//! Returns the index of one of the controllers drawn uniformly at random
	unsigned getUniformRandomIndex() const {
		float dice = Engine::GeneralState::statistics().getUniformDistValue();
		float accum = 0.0;
		unsigned i = 0;
		float step = 1.0 / _controllerProbabilities.size();
		for(; i < _controllerProbabilities.size(); ++i) {
			accum += step;
			if (dice < accum) break;
		}
		return i;
	}
	
	//! in-place normalization
	static void normalize(std::vector<float>& probabilities) {
		float sum = std::accumulate(probabilities.begin(), probabilities.end(), 0);
		for (unsigned i = 0; i < probabilities.size(); ++i) {
			probabilities[i] = probabilities[i] / sum;
		}
	}
};

}

#endif
