from string import Template

from .helper import mkdirp, write_code, load_file, DISTRIBUTION_BASE_DIR
from src import taskgen


class SequentialTaskgen(taskgen.Taskgen):
    """
    A simple task generator to execute experiments sequentially.
    """

    def print_task(self, lines):

        log_dir = self.experiment.directory + '/run'
        mkdirp(log_dir)

        task_filename = log_dir + "/run-task.sh"
        write_code(self.generate_run_task(lines), task_filename)

        runner = Template(load_file('tpl/task-iterator.sh')).substitute(
            nlines=len(lines),
            runtask=task_filename
        )

        runner_filename = log_dir + '/run.sh'
        write_code(runner, runner_filename)

        print("A total of {} sub-tasks have been generated. Run task '{}' by executing: \n\n\t{}\n"
              .format(len(lines), self.experiment.name, runner_filename))