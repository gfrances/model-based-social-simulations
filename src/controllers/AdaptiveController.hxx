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
	std::vector<float> _controllerProbabilities;
	
public:
	AdaptiveController(const ControllerSet& controllers, const std::vector<float>& probabilities) :
		_controllers(controllers), _controllerProbabilities(probabilities)
	{
		if (controllers.size() != probabilities.size() || std::accumulate(probabilities.begin(), probabilities.end(), 0) != 1) {
			throw std::runtime_error("Wrong probability distribution over the different strategies");
		}
	}
	virtual ~AdaptiveController() {}
	
	virtual Engine::Action* selectAction(const ModelAgent& agent) const {
		// We randomly choose which controller will select the action and then delegate the decision to it.
		float dice = Engine::GeneralState::statistics().getUniformDistValue();
		float accum = 0.0;
		unsigned i = 0;
		for(; i < _controllerProbabilities.size(); ++i) {
			accum += _controllerProbabilities[i];
			if (dice < accum) {
				break;
			}
		}
		return _controllers[i]->selectAction(agent);
	}
	
	virtual std::string getType() const { return "adaptive"; }
};

}

#endif
