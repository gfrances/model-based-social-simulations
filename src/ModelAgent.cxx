
#include "ModelAgent.hxx"
#include <Statistics.hxx>
#include <World.hxx>
#include <GeneralState.hxx>

namespace Model
{

ModelAgent::ModelAgent(unsigned id, std::shared_ptr<AgentController> controller)
	: Agent("ModelAgent_" + id), _controller(controller), _resources(INITIAL_RESOURCES)
{
}


ModelAgent::~ModelAgent()
{
}

void ModelAgent::selectActions()
{
}

/**
 * 
 */
Engine::Action* ModelAgent::getLastSelectedAction() {
	assert(_actions.size() == 1 && _actions.front()); // Only one action selected, and not null.
	return _actions.front();
}

void ModelAgent::updateState()
{
    move();
    collect();
    consume();
    checkStarvation();
}

void ModelAgent::collect()
{
    _resources += getWorld()->getValue("resources", getPosition());
    getWorld()->setValue("resources", getPosition(), 0);
}

void ModelAgent::move()
{
    Engine::Point2D<int> newPosition = getPosition();
    newPosition._x += Engine::GeneralState::statistics().getUniformDistValue(-1,1);
    newPosition._y += Engine::GeneralState::statistics().getUniformDistValue(-1,1);
    if(getWorld()->checkPosition(newPosition))
    {
       setPosition(newPosition);
    }
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

