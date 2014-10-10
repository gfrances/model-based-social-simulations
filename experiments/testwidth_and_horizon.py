#! /usr/bin/env python
# -*- coding: utf-8 -*-
from src.helper import make_filename
from src.experiment import AggregateExperiment, MDPAgentConfiguration, SingleExperiment, RandomAgentConfiguration
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

        base_params = dict(timesteps=200, runs=1,
                           simulation_map='r25_i0',
                           consumption=2,
                           agent_reproduction=False,
                           agent_position=initial_position)

        # The random agent baseline
        random = RandomAgentConfiguration(population=1)
        label = make_filename(agent='random', run=run)
        exp.add_single(SingleExperiment(agents=[random],
                                        label=label,
                                        **base_params))

        for horizon in [2, 4, 6, 8, 10, 12]:
            for width in [50, 100, 500, 1000, 5000]:
                agent = MDPAgentConfiguration(population=1, horizon=horizon, width=width)
                label = make_filename(agent='mdp', width=width, horizon=horizon, run=run)
                exp.add_single(SingleExperiment(agents=[agent], label=label, **base_params))

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
