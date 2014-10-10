#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse

from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from src.sge_taskgen import SGETaskgen
from random import randint


def main():
    """
    This is a simple example: an experiment executing a number of simulations with homogeneous MDP agents,
    each batch of a few runs with the same horizon but different width.
    """
    exp = AggregateExperiment(parse_arguments())

    runs = 50
    for run in range(0, runs):
        x, y = randint(0, 49), randint(0, 49)
        initial_position = "{},{}".format(x, y)

        for width in [10, 50, 100, 200, 500, 1000, 5000, 10000]:
            agent = MDPAgentConfiguration(population=1, horizon=10, width=width)

            exp.add_single(SingleExperiment(timesteps=200, runs=1,
                                            simulation_map='r25_i0',
                                            consumption=2,
                                            agent_reproduction=False,
                                            agent_position=initial_position,
                                            label="width_{}_run_{}".format(width, run),
                                            agents=[agent]))

    exp.bootstrap()

    t = SGETaskgen(exp)
    t.run()


def parse_arguments():
    opt = Options()
    opt.name = "width_sa"
    opt.timeout = 1*3600
    opt.mem = 8
    return opt


class Options(object):
    pass


if __name__ == "__main__":
    main()
