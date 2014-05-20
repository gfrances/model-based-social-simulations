
#include <ModelAgent.hxx>
#include <Statistics.hxx>
#include <World.hxx>
#include <GeneralState.hxx>

namespace Model
{

ModelAgent::ModelAgent( const std::string & id ) : Agent(id), _resources(5)
{
}

ModelAgent::~ModelAgent()
{
}

void ModelAgent::selectActions()
{
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

