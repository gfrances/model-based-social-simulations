#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse

# from experiments.src.exp import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from src.sge_taskgen import SGETaskgen
from src.sequential_taskgen import SequentialTaskgen


def main():
    """
    This is a simple example: an experiment executing a number of simulations with homogeneous MDP agents,
    each batch of ten runs with a different width.
    """
    exp = AggregateExperiment(parse_arguments())

    for width in range(200, 1000, 200):
        agent = MDPAgentConfiguration(population=10, horizon=10, width=width)
        exp.add_single(SingleExperiment(timesteps=100, runs=5,
                                        simulation_map='r25_i0',
                                        label="width_{}".format(width),
                                        agents=[agent]))

    exp.bootstrap()

    t = SGETaskgen(exp)
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
