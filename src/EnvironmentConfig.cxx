
#include <EnvironmentConfig.hxx>
#include <Exception.hxx>

namespace Model
{

EnvironmentConfig::EnvironmentConfig() : _numAgents(0), _size(0, 0), _fileName("none"), _loadResourcesFromFile(false)
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
    std::string method;
    retrieveAttributeMandatory(element, "method", method);
    if(method=="file")
    {
        _loadResourcesFromFile = true;
        TiXmlElement * element2 = element->FirstChildElement("file");
        retrieveAttributeMandatory(element2, "fileName", _fileName);
    }
    else if(method=="random")
    {
        _loadResourcesFromFile = false;
        TiXmlElement * element2 = element->FirstChildElement("random");
        retrieveAttributeMandatory(element2, "range", _range);
    }
    else
    {	
        std::stringstream oss;
		oss << "EnvironmentConfig::extractParticularAttribs - unknown resource generation method: " << method;
		throw Engine::Exception(oss.str());
    }
}
	
const Engine::Size<int> & EnvironmentConfig::getSize() const
{
	return _size;
}
    
bool EnvironmentConfig::loadResourcesFromFile() const
{
    return _loadResourcesFromFile;
}

} // namespace Model

