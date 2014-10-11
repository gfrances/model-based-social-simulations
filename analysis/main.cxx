

#include <Exception.hxx>
#include <SimulationRecord.hxx>

#include <analysis/GlobalAgentStats.hxx>
#include <analysis/GlobalRasterStats.hxx>
#include <analysis/AgentMean.hxx>
#include <analysis/AgentStdDev.hxx>
#include <analysis/AgentSum.hxx>
#include <analysis/RasterMean.hxx>
#include <analysis/RasterSum.hxx>
#include <analysis/AgentNum.hxx>
#include <analysis/AgentHDFtoSHP.hxx>
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char *argv[])
{
// 	if(argc!=4) throw Engine::Exception("USAGE: analysis file.h5 agent.csv rasters.csv");
	if(argc != 3) throw Engine::Exception("USAGE: ./analysis.bin file.h5 output_dir");

	Engine::SimulationRecord simRecord(1, false);
	simRecord.loadHDF5(argv[1], true, true);

	// Process the agent data
	PostProcess::GlobalAgentStats agentResults;
	agentResults.addAnalysis(std::make_shared<PostProcess::AgentNum>());
	agentResults.addAnalysis(std::make_shared<PostProcess::AgentMean>("resources"));
// 	agentResults.addAnalysis(std::make_shared<PostProcess::AgentStdDev>("resources"));
// 	agentResults.addAnalysis(std::make_shared<PostProcess::AgentSum>("resources"));
// 	agentResults.addAnalysis(std::make_shared<PostProcess::AgentMean>("children"));
	
// 	const std::vector<std::string> TYPES = {"all", "mdp", "random"};
// 	const std::vector<std::string> TYPES = {"mdp", "random"};
	const std::vector<std::string> TYPES = {"mdp", "random", "lazy", "greedy"};
	for (const auto& type:TYPES) {
		std::string output(std::string(argv[2]) + "/agent-" + type + ".csv");
		agentResults.apply(simRecord, output, type);
	}
	
	// Process the raster data
// 	PostProcess::GlobalRasterStats rasterResults;
// 	rasterResults.addAnalysis(std::make_shared<PostProcess::RasterMean>());
// 	rasterResults.addAnalysis(std::make_shared<PostProcess::RasterSum>());
// 	rasterResults.apply(simRecord, "rasters.csv", "resources");
	
	
	return 0;
}

