
#ifndef __EnvironmentConfig_hxx__
#define __EnvironmentConfig_hxx__

#include <Config.hxx>
#include <Size.hxx>

namespace Model
{

enum class AgentControllerType { random, MDP, lazy, maximizer, ruleBased };

class ControllerConfig {
public:
		AgentControllerType controllerType;
		unsigned width;
		unsigned horizon;
		float explorationBonus;
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

