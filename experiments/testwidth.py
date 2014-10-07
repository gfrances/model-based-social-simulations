#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse

from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from src.sge_taskgen import SGETaskgen


def main():
    """
    This is a simple example: an experiment executing a number of simulations with homogeneous MDP agents,
    each batch of a few runs with a the same horizon but different width.
    """
    exp = AggregateExperiment(parse_arguments())

    for width in [100, 500, 1000, 5000, 10000]:
        agent = MDPAgentConfiguration(population=10, horizon=10, width=width)
        exp.add_single(SingleExperiment(timesteps=200, runs=5,
                                        simulation_map='r25_s50x50',
                                        consumption_factor=3,
                                        agent_reproduction=True,
                                        agent_position="",  # random position
                                        label="width_{}".format(width),
                                        agents=[agent]))

    exp.bootstrap()

    t = SGETaskgen(exp)
    t.run()


def parse_arguments():
    opt = Options()
    opt.name = "width"
    opt.timeout = 6*3600
    opt.mem = 8
    return opt


class Options(object):
    pass


if __name__ == "__main__":
    main()
