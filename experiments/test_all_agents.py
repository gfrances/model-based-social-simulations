#! /usr/bin/env python
# -*- coding: utf-8 -*-

import itertools
from src.helper import make_filename
from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment, RandomAgentConfiguration, \
    LazyAgentConfiguration, GreedyAgentConfiguration
from src.sge_taskgen import SGETaskgen


def do_experiment(agent_names, args):
    """
        Test the relative performance of different agent types.
    """
    exp = AggregateExperiment(args)

    runs = range(0, 10)
    population = 10

    autocorrelations = [1, 10, 25]
    map_instances = list(range(1, 6))  # 5 different map instances
    consumptions = [2, 3, 4, 5]

    agents = dict(
        mdp=MDPAgentConfiguration(population=population, horizon=8, width=1000),
        lazy=LazyAgentConfiguration(population=population, alpha=0.8),
        random=RandomAgentConfiguration(population=population),
        greedy=GreedyAgentConfiguration(population=population),
        # null=MotionlessAgentConfiguration(population=population)
    )

    agents = {k: v for k, v in agents.items() if k in agent_names}  # Filter out undesired agents

    for agent_name, agent in agents.items():

        for autocorrelation, map_instance, consumption, run in \
                itertools.product(autocorrelations, map_instances, consumptions, runs):

            map_filename = 'r{}_i{}'.format(autocorrelation, map_instance)

            params = dict(timesteps=1000,
                          runs=1,
                          agent_reproduction=True,
                          agent_position='',
                          simulation_map=map_filename,
                          consumption=consumption,
                          agents=[agent])

            label = make_filename(autocorrelation=autocorrelation,
                                  map=map_instance,
                                  consumption=consumption,
                                  agent=agent_name,
                                  run=run)

            exp.add_single(SingleExperiment(label=label, **params))

    exp.bootstrap()

    t = SGETaskgen(exp)
    t.run()


def parse_arguments(timeout):
    opt = Options()
    opt.name = "test_all"
    opt.timeout = timeout
    opt.mem = 8
    return opt


class Options(object):
    pass


def main():
    do_experiment(['lazy', 'random', 'greedy'], parse_arguments(timeout=100))  # 100sec. are enough for the cheap agents
    do_experiment(['mdp'], parse_arguments(timeout=12*3600))  # We need much more time for the MDP agent


if __name__ == "__main__":
    main()
