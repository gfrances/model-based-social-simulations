#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse

# from experiments.src.exp import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from src.sequential_taskgen import SequentialTaskgen
from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment, LazyAgentConfiguration


def main():
    """
    A simple single-run experiment comparing the performance of 10 MDP agents with that of 10 0.7-lazy agents.
    """
    exp = AggregateExperiment(parse_arguments())

    mdp_agent = MDPAgentConfiguration(population=10, horizon=6, width=1000)
    lazy_agent = LazyAgentConfiguration(population=10, alpha=0.7)

    exp.add_single(SingleExperiment(timesteps=200, runs=1, simulation_map='r25_i0',
                                    label="all", agents=[mdp_agent, lazy_agent]))

    exp.bootstrap()

    t = SequentialTaskgen(exp)
    t.run()


def parse_arguments():
    parser = argparse.ArgumentParser(description='Generate experiment task runners.')
    parser.add_argument("--name", required=True, help='The name/ID we want to give to the experiment', default='')
    parser.add_argument("--timeout", required=True, help='Maximum timeout allowed, in seconds', type=int)
    parser.add_argument("--mem", help='Maximum memory allowed, in GB', default='2', type=int)
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    main()
