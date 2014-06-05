
#include "ModelAgent.hxx"
#include <Statistics.hxx>
#include <World.hxx>
#include <GeneralState.hxx>

namespace Model
{

ModelAgent::ModelAgent(unsigned id, std::shared_ptr<AgentController> controller)
	: Agent("ModelAgent_" + id), _controller(controller), _resources(INITIAL_RESOURCES)
{}

ModelAgent::~ModelAgent() {}

void ModelAgent::selectActions()
{
	assert(_actions.size()==0);
// 	_actions.clear();
	_actions.push_back(_controller->selectAction(*this));
}

void ModelAgent::updateState()
{
    collect();
    consume();
    checkStarvation();
}

void ModelAgent::collect()
{
    _resources += getWorld()->getValue("resources", getPosition());
    getWorld()->setValue("resources", getPosition(), 0);
}


void ModelAgent::consume()
{
    _resources--;
}

void ModelAgent::checkStarvation()
{
    if(_resources>=0)
    {
        return;
    }
    remove();
}

void ModelAgent::registerAttributes()
{
	registerIntAttribute("resources");
}

void ModelAgent::serialize()
{
	serializeAttribute("resources", _resources);
}

void ModelAgent::setResources( int resources )
{
	_resources = resources;
}

int ModelAgent::getResources() const
{
	return _resources;
}

} // namespace Model

