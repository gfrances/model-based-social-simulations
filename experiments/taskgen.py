#! /usr/bin/env python
# -*- coding: utf-8 -*-
import argparse
import json
import os
import time
from string import Template
import itertools

from experiments.helper import print_task, HOMEPATH, mkdirp, save_file

MAPS_DIR = os.path.abspath('../maps')  # This is the place were the benchmark maps reside.
RESULTS_DIR = os.path.abspath(HOMEPATH + '/projects/simulpast/experiments')

CONTROLLER_CONFIG = dict(
    mdp='<controller type= "MDP" population="${population}" horizon="${horizon}" width="${width}" explorationBonus="${bonus}"/>',
    random='<controller type= "random" population="${population}"/>',
    lazy='<controller type= "lazy" population="${population}" alpha="${alpha}"/>'
)


def parse_arguments():
    parser = argparse.ArgumentParser(description='Generate experiment task runners.')
    parser.add_argument("--name", required=True, help='The name/ID we want to give to the experiment', default='')
    parser.add_argument("--timeout", required=True, help='Maximum timeout allowed, in seconds', type=int)
    parser.add_argument("--mem", help='Maximum memory allowed, in GB', default='2', type=int)
    args = parser.parse_args()
    return args


def gen_task_options(solver, timeout, setname):
    n = 1
    code = ''

    for exec_line in solver(timeout=timeout, setname=setname):
        code += "{case})  {exec_line} ;;".format(
            case=n,
            exec_line=exec_line
        ) + '\n\n'
        n += 1

    return n-1, code


def get_agent_configuration_string(agent_type, **kwargs):
    return Template(CONTROLLER_CONFIG[agent_type]).substitute(kwargs)


class AgentConfiguration(object):
    def __init__(self, population):
        self.population = population

    def get_type(self):
        raise RuntimeError("Method AgentConfiguration.get_type needs to be subclassed")

    def generate_config(self):
        return Template(CONTROLLER_CONFIG[self.get_type()]).substitute(self.__dict__)


class MDPAgentConfiguration(AgentConfiguration):
    def __init__(self, population, width, horizon, bonus=10):
        super().__init__(population)
        self.width = width
        self.horizon = horizon
        self.bonus = bonus

    def get_type(self):
        return 'mdp'


class RandomAgentConfiguration(AgentConfiguration):
    def get_type(self):
        return 'random'


class SingleExperiment(object):
    """
    A single experiment is an experiment with one single configuration file, but that might have several runs.
    """
    def __init__(self, timesteps, simulation_map, label, runs, agents=None):
        self.timesteps = timesteps
        self.map = simulation_map
        self.runs = runs
        self.agents = agents if agents else []
        self.label = label
        self.directory = None

    def add_agent_type(self, config):
        assert isinstance(config, AgentConfiguration)
        self.agents.append(config)

    def get_map_filename(self):
        return MAPS_DIR + '/' + self.map

    def bootstrap(self, base_directory):
        self.directory = base_directory + '/' + self.label

        # Create the dirs and write the config files corresponding to each run
        for run in range(self.runs):
            run_dir = self.directory + '/run_' + str(run)
            output_dir = run_dir + '/output'
            logs = run_dir + '/logs'
            mkdirp(output_dir)
            mkdirp(logs)
            config = self.generate_config(
                output=output_dir + '/model.h5',
                logs=logs
            )
            save_file(run_dir + '/config.xml', config)

    def generate_config(self, output, logs):
        """ Generates and saves the config.xml file corresponding to the current object parameters """

        with open('tpl/config.xml', "r") as f:
            tpl = Template(f.read())

        # Flesh out the template with both the elements of the current object plus the received args
        return tpl.substitute(
            timesteps=self.timesteps,
            map=self.map,
            output=output,
            logs=logs,
            agent_config='\n\t\t'.join(agent.generate_config() for agent in self.agents)
        )


class AggregateExperiment(object):
    """
    An AggregateExperiment might involve running several SingleExperiments with different configurations,
    e.g. with varying widths of the simulation's MDP agents, etc.
    """
    def __init__(self, args):
        self.timestamp = time.strftime("%Y%m%d_%H%M")
        self.timeout = args.timeout
        self.mem = '{}GB'.format(args.mem)
        self.name = '.'.join([args.name, str(args.timeout) + 'sec', self.mem, self.timestamp])
        self.directory = RESULTS_DIR + '/' + self.name
        self.singles = []

    def add_single(self, single):
        assert isinstance(single, SingleExperiment)
        self.singles.append(single)
        single.bootstrap(self.directory)

    def bootstrap(self):
        """ Bootstraps an AggregateExperiment by creating the necessary directory structure for its results  """
        print("Bootstrapping experiment {} on directory {}".format(self.name, self.directory))
        for single in self.singles:
            single.bootstrap(self.directory)

        # Write all the experiment relevant info
        save_file(self.directory + '/experiment.json', self.dump())

    def __str__(self):
            return "Experiment {}".format(self.name)

    def dump(self):
        return json.dumps(self, default=lambda o: o.__dict__)


class Taskgen(object):
    def __init__(self, experiment):
        assert isinstance(experiment, AggregateExperiment)

    def print(self):
        pass
        # self.noptions, self.task_options = gen_task_options(solvers.get_solver(args.solver).runner,
        #                                                     args.timeout,
        #                                                     args.set)
        # print_task(self)


def main():
    args = parse_arguments()
    exp = AggregateExperiment(args)

    # run = Run(100, 'r25_s50x50')
    # run.add_agent_type(MDPAgentConfiguration(population=10, horizon=10, width=500))
    # run.add_agent_type(RandomAgentConfiguration(population=10))

    for width in range(100, 1000, 100):
        agent = MDPAgentConfiguration(population=10, horizon=10, width=width)
        exp.add_single(SingleExperiment(timesteps=100, runs=10,
                                        simulation_map='r25_s50x50',
                                        label="width_{}".format(width),
                                        agents=[agent]))
    exp.bootstrap()

    t = Taskgen(exp)
    t.print()

if __name__ == "__main__":
    main()
