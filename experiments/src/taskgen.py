import os
from string import Template

from .experiment import AggregateExperiment
from .helper import HOMEPATH, mkdirp, write_code, load_file, DISTRIBUTION_BASE_DIR


MAIL = 'guillem.frances@upf.edu'
SIMULPAST_DIR = os.path.abspath(HOMEPATH + '/projects/simulpast')
SIMULATOR = '${SIMULPAST_DIR}/model-based-social-simulations/src/simulation.bin'


class Taskgen(object):
    """
    The task generator interface
    """
    def __init__(self, experiment):
        assert isinstance(experiment, AggregateExperiment)
        self.experiment = experiment

    def run(self):
        lines = self.generate_task()
        self.print_task(lines)

    def generate_task(self):
        lines = []
        configs = [config for single in self.experiment.singles for config in single.configs]

        for i, config in enumerate(configs, 1):
            lines.append("{case})  {bin} {config} ;;".format(
                case=i,
                bin=SIMULATOR,
                config=config.replace(SIMULPAST_DIR, '${SIMULPAST_DIR}')
            ))

        return lines

    def print_task(self, lines):
        raise RuntimeError("This method must be subclassed")

