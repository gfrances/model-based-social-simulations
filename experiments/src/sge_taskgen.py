from string import Template

from .helper import mkdirp, write_code, load_file, DISTRIBUTION_BASE_DIR
from src import taskgen


class SGETaskgen(taskgen.Taskgen):
    """
    A task generator specific for the Sun Grid Engine.
    """

    def print_task(self, lines):
        log_dir = self.experiment.directory + '/run'
        mkdirp(log_dir)

        task_filename = log_dir + "/run-task.sh"

        write_code(self.generate_run_task(lines), task_filename)

        qsub_line = "qsub -t 1-{noptions} -l h_rt={rt} -l h_cpu={timeout} -l h_vmem={mem}G \\\n\t\t" \
                    " -e  {log_dir} \\\n\t\t -o {log_dir} \\\n\t\t" \
                    " -N {task_name} -cwd -m as -M {mail} -hard -binding linear:16 \\\n\t\t {task}".format(
                        noptions=len(lines),
                        mail=taskgen.MAIL,
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
