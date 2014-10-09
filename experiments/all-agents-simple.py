#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse

# from experiments.src.exp import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from src.sequential_taskgen import SequentialTaskgen
from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment, LazyAgentConfiguration, \
    RandomAgentConfiguration, GreedyAgentConfiguration, MotionlessAgentConfiguration


def main():
    """
    A simple single-run experiment comparing the performance of 10 units of each of our agent types.
    """
    exp = AggregateExperiment(parse_arguments())

    pop = 10
    mdp = MDPAgentConfiguration(population=pop, horizon=6, width=1000)
    lazy = LazyAgentConfiguration(population=pop, alpha=0.7)
    random = RandomAgentConfiguration(population=pop)
    greedy = GreedyAgentConfiguration(population=pop)
    motionless = MotionlessAgentConfiguration(population=pop)

    exp.add_single(SingleExperiment(timesteps=200, runs=1, simulation_map='r25_i0',
                                    label="all", agents=[mdp, lazy, random, greedy, motionless]))

    exp.bootstrap()

    t = SequentialTaskgen(exp)
    t.run()


def parse_arguments():
    parser = argparse.ArgumentParser(description='Generate experiment task runners.')
    parser.add_argument("--name", help='The name/ID we want to give to the experiment', default='all-agents')
    parser.add_argument("--timeout", help='Maximum timeout allowed, in seconds', type=int, default='0')
    parser.add_argument("--mem", help='Maximum memory allowed, in GB', default='0', type=int)
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    main()
