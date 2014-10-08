
#ifndef __EnvironmentConfig_hxx__
#define __EnvironmentConfig_hxx__

#include <vector>
#include <memory>
#include <Config.hxx>
#include <Size.hxx>
#include <Exception.hxx>
#include <Point2D.hxx>

namespace Model
{

class ControllerConfig {
public:
	std::string type;
	unsigned width;
	unsigned horizon;
	float explorationBonus;
	unsigned population;
	
	float alpha; // alpha parameter for the aplha-lazy controllers.

	const std::string& getType() const { return type; }
	unsigned getWidth() const { return width; }
	unsigned getHorizon() const { return horizon; }
	float getExplorationBonus() const { return explorationBonus; }
	unsigned getPopulation() const { return population; }
};


class EnvironmentConfig : public Engine::Config
{
public:
	typedef std::shared_ptr<const EnvironmentConfig> cptr;
	typedef std::shared_ptr<Engine::Point2D<int>> PointPtr;
	
protected:
	static const std::vector<std::string> ALLOWED_CONTROLLERS;
	
	Engine::Size<int> _size;
    std::string map;
	std::string _logdir;
	std::vector<ControllerConfig> _controllers;
	float _consumptionFactor;
	bool _agentReproduction;
	PointPtr _initialAgentPosition;
	
	//! Loads the configurations for all the agents controllers
	void loadControllerParams();
	
	//! Loads the particular configuration of a single controller
	void loadSingleControllerConfig(TiXmlElement* element);
	
	//! Parses the (optional) initial position of the agents
	PointPtr parseInitialPosition();
	
public:
	
	EnvironmentConfig(const std::string& filename);
	virtual ~EnvironmentConfig();

	void loadParams();
	
	const Engine::Size<int>& getSize() const { return _size; };
	
	float getConsumptionFactor() const { return _consumptionFactor; }
	
	bool doAgentsReproduce() const { return _agentReproduction; }
	
	const PointPtr getInitialAgentPosition() const { return _initialAgentPosition; }
	
	const std::string& getMap() const { return map; }
	
	const std::vector<ControllerConfig>& getControllerConfigurations() const { return _controllers; }
	
	const std::string& getLogDir() { return _logdir; }
};

} // namespace Model

#endif // __EnvironmentConfig_hxx__

