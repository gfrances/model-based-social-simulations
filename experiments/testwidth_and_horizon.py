#! /usr/bin/env python
# -*- coding: utf-8 -*-
from src.helper import make_filename
from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment
from src.sge_taskgen import SGETaskgen
from random import randint


def main():
    """
    """
    exp = AggregateExperiment(parse_arguments())

    runs = 20
    for run in range(0, runs):
        x, y = randint(0, 49), randint(0, 49)
        initial_position = "{},{}".format(x, y)

        for horizon in [2, 4, 6, 8]:
            for width in [50, 100, 500, 1000]:
                agent = MDPAgentConfiguration(population=1, horizon=horizon, width=width)

                exp.add_single(SingleExperiment(timesteps=200, runs=1,
                                                simulation_map='r25_s50x50',
                                                consumption_factor=2,
                                                agent_reproduction=False,
                                                agent_position=initial_position,
                                                label=make_filename(width=width, horizon=horizon, run=run),
                                                agents=[agent]))

    exp.bootstrap()

    t = SGETaskgen(exp)
    t.run()


def parse_arguments():
    opt = Options()
    opt.name = "width_sa"
    opt.timeout = 1800
    opt.mem = 8
    return opt


class Options(object):
    pass


if __name__ == "__main__":
    main()
