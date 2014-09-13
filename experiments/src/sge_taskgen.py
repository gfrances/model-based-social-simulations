import os
from string import Template

from .experiment import AggregateExperiment
from .helper import HOMEPATH, mkdirp, write_code, load_file, DISTRIBUTION_BASE_DIR


MAIL = 'guillem.frances@upf.edu'
SIMULPAST_DIR = os.path.abspath(HOMEPATH + '/projects/simulpast')
SIMULATOR = '${SIMULPAST_DIR}/model-based-social-simulations/src/simulation.bin'


class Taskgen(object):
    """
    A task generator specific for the Sun Grid Engine.
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

        tpl = Template(load_file('tpl/run-task.sh'))
        task_code = tpl.substitute(
            task_options='\n'.join(lines),
            simulpast_dir=SIMULPAST_DIR
        )

        log_dir = DISTRIBUTION_BASE_DIR + "/experiments/logs/" + self.experiment.name
        mkdirp(log_dir)

        task_filename = log_dir + "/run-task.sh"

        write_code(task_code, task_filename)

        qsub_line = "qsub -t 1-{noptions} -l h_rt={rt} -l h_cpu={timeout} -l h_vmem={mem}G \\\n\t\t" \
                    " -e  {log_dir} \\\n\t\t -o {log_dir} \\\n\t\t" \
                    " -N {task_name} -cwd -m as -M {mail} -hard -binding linear:16 \\\n\t\t {task}".format(
                        noptions=len(lines),
                        mail=MAIL,
                        task=task_filename,
                        task_name=self.experiment.name,
                        # The CPU time HARD timeout  -  leave some small extra time
                        timeout=self.experiment.timeout + 5*60,

                        # The REAL time HARD timeout - Leave some good extra time.
                        rt=max(self.experiment.timeout + 15*60, 3*3600),
                        mem=self.experiment.mem,
                        log_dir=log_dir
                    )
        qsub_filename = log_dir + '/qsub-task.sh'
        write_code(qsub_line, qsub_filename)

        print('A total of {} sub-tasks have been generated'.format(len(lines)))
        print("Run task '{0}' by executing the following: \n\t{1}".format(
            self.experiment.name, qsub_filename))
        print('\n\nOr, alternatively, run:\n\t' + qsub_line + '\n')
        print("Add the option -verify to see what would be submitted.")
