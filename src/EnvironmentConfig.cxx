
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>

namespace Model
{

EnvironmentConfig::EnvironmentConfig() : _numAgents(0), _size(0, 0), _fileName("none")
{
}

EnvironmentConfig::~EnvironmentConfig()
{
}

void EnvironmentConfig::extractParticularAttribs(TiXmlElement * root)
{
	TiXmlElement * element = root->FirstChildElement("size");
	retrieveAttributeMandatory( element, "width", _size._width);
	retrieveAttributeMandatory( element, "height", _size._height);

    element = root->FirstChildElement("environment");
    retrieveAttributeMandatory(element, "fileName", _fileName);
    
    element = root->FirstChildElement("agents");
    retrieveAttributeMandatory(element, "initPop", _numAgents);
}
	
const Engine::Size<int> & EnvironmentConfig::getSize() const
{
	return _size;
}

} // namespace Model

