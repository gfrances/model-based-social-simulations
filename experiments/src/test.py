#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse

from .exp import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from .sge_taskgen import Taskgen


def main():
    """
    This is a simple example: an experiment executing a number of simulations with homogeneous MDP agents,
    each batch of ten runs with a different width.
    """
    exp = AggregateExperiment(parse_arguments())

    for width in range(100, 1000, 100):
        agent = MDPAgentConfiguration(population=10, horizon=10, width=width)
        exp.add_single(SingleExperiment(timesteps=100, runs=10,
                                        simulation_map='r25_s50x50',
                                        label="width_{}".format(width),
                                        agents=[agent]))

    exp.bootstrap()

    Taskgen(exp).print()


def parse_arguments():
    parser = argparse.ArgumentParser(description='Generate experiment task runners.')
    parser.add_argument("--name", required=True, help='The name/ID we want to give to the experiment', default='')
    parser.add_argument("--timeout", required=True, help='Maximum timeout allowed, in seconds', type=int)
    parser.add_argument("--mem", help='Maximum memory allowed, in GB', default='2', type=int)
    args = parser.parse_args()
    return args

if __name__ == "__main__":
    main()