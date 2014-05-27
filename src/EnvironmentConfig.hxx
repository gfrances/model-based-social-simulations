
#ifndef __EnvironmentConfig_hxx__
#define __EnvironmentConfig_hxx__

#include <Config.hxx>
#include <Size.hxx>

namespace Model
{

class EnvironmentConfig : public Engine::Config
{	
	int _numAgents;
    // range of the spatial autocorrelation
    int _range;
	Engine::Size<int> _size;
    std::string _fileName;
    bool _loadResourcesFromFile;
public:
	EnvironmentConfig();
	virtual ~EnvironmentConfig();

	void extractParticularAttribs(TiXmlElement *pRoot);

	const Engine::Size<int> & getSize() const;

	friend class Environment;
    bool loadResourcesFromFile() const;
};

} // namespace Model

#endif // __EnvironmentConfig_hxx__

