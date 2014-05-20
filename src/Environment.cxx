
#include <Environment.hxx>

#include <EnvironmentConfig.hxx>
#include <ModelAgent.hxx>
#include <Simulation.hxx>
#include <DynamicRaster.hxx>
#include <Point2D.hxx>
#include <GeneralState.hxx>
#include <RInside.h>

namespace Model 
{

Environment::Environment(const EnvironmentConfig & config, Engine::Simulation & simulation, Engine::Scheduler * scheduler ) : World(simulation, scheduler, false), _config(config)
{
}

Environment::~Environment()
{
}

void Environment::createRasters()
{	
    registerDynamicRaster("resources", true);
	getDynamicRaster("resources").setInitValues(0, 10, 0);
    RInside R(0,0);
    std::string txt = "suppressMessages(library(gstat))";
    R.parseEvalQ(txt);
    txt = "suppressMessages(library(sp))";
    R.parseEvalQ(txt);
	
    std::stringstream oss;
    oss << "xy <- expand.grid(1:"<<_config._size._width<<", 1:"<<_config._size._height<<")";
    R.parseEval(oss.str());
    txt = "names(xy) <- c('x','y')";
    R.parseEval(txt);
    std::stringstream modelCommand;
    modelCommand << "yy <- predict(gstat(formula=z~1, locations=~x+y, dummy=T, beta=c(1), model=vgm(psill=0.025, range=" << _config._range << ", model='Exp'), nmax=20), xy, nsim=1)";
    R.parseEval(modelCommand.str());
    txt ="gridded(yy) = ~x+y";
    R.parseEval(txt);
    txt ="resources <- as.matrix(yy)";
    R.parseEval(txt);
    // normalize
    txt ="resources <- (resources-min(resources))/(max(resources)-min(resources))";
    Rcpp::NumericMatrix resources= R.parseEval(txt);
	for(auto index:getBoundaries())
    {
        getDynamicRaster("resources").setMaxValue(index, 10.0f*resources(index._x, index._y));
    }
	updateRasterToMaxValues("resources");
}

void Environment::createAgents()
{
	for(int i=0; i<_config._numAgents; i++)
	{
		if((i%getNumTasks())==getId())
		{
			std::ostringstream oss;
			oss << "ModelAgent_" << i;
			ModelAgent * agent = new ModelAgent(oss.str());
			addAgent(agent);
			agent->setRandomPosition();
		}
	}
}

} // namespace Model

