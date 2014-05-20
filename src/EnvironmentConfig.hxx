
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
public:
	EnvironmentConfig();
	virtual ~EnvironmentConfig();

	void extractParticularAttribs(TiXmlElement *pRoot);

	const Engine::Size<int> & getSize() const;

	friend class Environment;
};

} // namespace Model

#endif // __EnvironmentConfig_hxx__

