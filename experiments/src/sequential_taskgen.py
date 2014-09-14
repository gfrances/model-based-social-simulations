from string import Template

from .helper import mkdirp, write_code, load_file, DISTRIBUTION_BASE_DIR
from src import taskgen


class SequentialTaskgen(taskgen.Taskgen):
    """
    A simple task generator to execute experiments sequentially.
    """

    def print_task(self, lines):

        tpl = Template(load_file('tpl/run-task.sh'))
        task_code = tpl.substitute(
            task_options='\n'.join(lines),
            simulpast_dir=taskgen.SIMULPAST_DIR
        )

        log_dir = DISTRIBUTION_BASE_DIR + "/experiments/logs/" + self.experiment.name
        mkdirp(log_dir)

        task_filename = log_dir + "/run-task.sh"
        write_code(task_code, task_filename)

        runner = Template(load_file('tpl/task-iterator.sh')).substitute(
            nlines=len(lines),
            runtask=task_filename
        )

        runner_filename = log_dir + '/run.sh'
        write_code(runner, runner_filename)

        print('A total of {} sub-tasks have been generated'.format(len(lines)))
        print("Run task '{0}' by executing the following: \n\t{1}".format(
            self.experiment.name, runner_filename))