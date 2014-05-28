
#ifndef __EnvironmentConfig_hxx__
#define __EnvironmentConfig_hxx__

#include <Config.hxx>
#include <Size.hxx>

namespace Model
{

class EnvironmentConfig : public Engine::Config
{	
	int _numAgents;
	Engine::Size<int> _size;
    std::string _fileName;
public:
	EnvironmentConfig();
	virtual ~EnvironmentConfig();

	void extractParticularAttribs(TiXmlElement *pRoot);
	const Engine::Size<int> & getSize() const;
	friend class Environment;
};

} // namespace Model

#endif // __EnvironmentConfig_hxx__

