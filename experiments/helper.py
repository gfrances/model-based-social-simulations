#
# Some helper methods
#
import glob
import os
import stat

MAIL = 'guillem.frances@upf.edu'

HOMEPATH = os.path.expanduser("~")

RESULTS_DIR = os.path.abspath('../results')
DOMAIN_FILENAME = "domain.pddl"

BMARK_PREFIX = HOMEPATH + '/projects/code/planning-vm-experiments/benchmarks/'


def get_subdirs(base, pattern='*'):
    """
    Return an iterator over pairs of all the (subdirectories, domain_dirname) of the base directory,
    skipping directories starting with '__'
    """
    for full in glob.iglob(base + '/' + pattern):
        _, last = os.path.split(full)
        if not last.startswith('__'):
            yield full, last

def mkdirp(directory):
    """" mkdir -p -like functionality """
    if not os.path.isdir(directory):
        os.makedirs(directory)


def load_file(filename):
    with open(filename, 'r') as f:
        content = f.read()
    return content


def save_file(filename, content):
    with open(filename, 'w') as f:
        f.write(content)


def countlines(filename):
    """ Count the lines of a file """
    return sum(1 for _ in open(filename, 'rb'))


def make_executable(filename):
    os.chmod(filename, os.stat(filename).st_mode | stat.S_IEXEC)


def write_code(code, filename):
    with open(filename, "w") as f:
        f.write(code)
    make_executable(filename)


def print_task(info):
    task_code = """#!/bin/bash

{pre}

case $SGE_TASK_ID in

{task_options}
*) echo error in SGE_TASK_ID:$SGE_TASK_ID ;;

esac
""".format(task_options=info.task_options, pre=info.pre if hasattr(info, 'pre') else '')

    log_dir = "./experiments/" + info.task_name
    mkdirp(log_dir)

    task_filename = log_dir + "/run-task.sh"

    write_code(task_code, task_filename)

    qsub_line = "qsub -t 1-{noptions} -l h_rt={rt} -l h_cpu={timeout} -l h_vmem={mem}G " \
                " -e  {log_dir} -o {log_dir} " \
                "-N {task_name} -cwd -m as -M {mail} -hard -binding linear:16 {task}".format(
                    noptions=info.noptions,
                    mail=MAIL,
                    task=task_filename,
                    task_name=info.task_name,
                    timeout=info.timeout + 5*60,  # The CPU time HARD timeout  -  leave some small extra time
                    rt=max(info.timeout + 15*60, 3*3600),  # The REAL time HARD timeout - Leave some good extra time.
                    mem=info.mem,
                    log_dir=log_dir
                )
    qsub_filename = log_dir + '/qsub-task.sh'
    write_code(qsub_line, qsub_filename)

    print('A total of {} sub-tasks have been generated'.format(info.noptions))
    print("Run task '{0}' by executing the following, available also in '{1}':".format(info.task_name, qsub_filename))
    print('\n\t' + qsub_line + '\n')
    print("Add the option -verify to see what would be submitted.")
