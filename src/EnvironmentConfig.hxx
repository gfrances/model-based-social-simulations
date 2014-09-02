
#ifndef __EnvironmentConfig_hxx__
#define __EnvironmentConfig_hxx__

#include <Config.hxx>
#include <Size.hxx>

namespace Model
{

enum class AgentControllerType { random, motionless, MDP, lazy, greedy, rule };

class ControllerConfig {
public:
	AgentControllerType controllerType;
	unsigned width;
	unsigned horizon;
	float explorationBonus;

	AgentControllerType getControllerType() const { return controllerType; }
	void setControllerType(const AgentControllerType& type) { controllerType = type; }
	unsigned getWidth() const { return width; }
	unsigned getHorizon() const { return horizon; }
	unsigned getExplorationBonus() const { return explorationBonus; }
};


class EnvironmentConfig : public Engine::Config
{
private:
	unsigned _numAgents;
	Engine::Size<int> _size;
    std::string _fileName;
	
	ControllerConfig controllerConfig;
	
	void retrieveControllerConfig(TiXmlElement* controller);
	
public:
	friend class Environment;
	EnvironmentConfig();
	virtual ~EnvironmentConfig();

	void extractParticularAttribs(TiXmlElement *pRoot);
	const Engine::Size<int> & getSize() const;
	
	const ControllerConfig& getControllerConfig() const { return controllerConfig; }
};

} // namespace Model

#endif // __EnvironmentConfig_hxx__

