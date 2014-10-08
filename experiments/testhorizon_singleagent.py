#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse

from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment, RandomAgentConfiguration
from src.sge_taskgen import SGETaskgen
from random import randint


def main():
    """
    """
    exp = AggregateExperiment(parse_arguments())

    runs = 50
    width = 1000

    for run in range(0, runs):
        x, y = randint(0, 49), randint(0, 49)
        initial_position = "{},{}".format(x, y)

        base_params = dict(timesteps=500, runs=1,
                           simulation_map='r25_s50x50',
                           consumption_factor=2,
                           agent_reproduction=False,
                           agent_position=initial_position)

        # The random agent baseline
        random = RandomAgentConfiguration(population=1)
        exp.add_single(SingleExperiment(agents=[random],
                                        label="random_run_{}".format(run),
                                        **base_params))

        # The MDP agents with varying horizons.
        for horizon in [2, 4, 6, 8, 10]:
            agent = MDPAgentConfiguration(population=1, horizon=horizon, width=width)
            exp.add_single(SingleExperiment(agents=[agent],
                                            label="horizon_{}_run_{}".format(horizon, run),
                                            **base_params))

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
