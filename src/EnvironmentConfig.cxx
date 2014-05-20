
#include <EnvironmentConfig.hxx>

namespace Model
{

EnvironmentConfig::EnvironmentConfig() : _numAgents(0), _size(0, 0)
{
}

EnvironmentConfig::~EnvironmentConfig()
{
}

void EnvironmentConfig::extractParticularAttribs(TiXmlElement * root)
{
	TiXmlElement * element = root->FirstChildElement("numAgents");
	retrieveAttributeMandatory( element, "value", _numAgents);
	element = root->FirstChildElement("size");
	retrieveAttributeMandatory( element, "width", _size._width);
	retrieveAttributeMandatory( element, "height", _size._height);

    element = root->FirstChildElement("environment");
    retrieveAttributeMandatory(element, "range", _range);
}
	
const Engine::Size<int> & EnvironmentConfig::getSize() const
{
	return _size;
}

} // namespace Model

