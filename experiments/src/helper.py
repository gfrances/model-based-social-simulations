#
# Some helper methods
#
import glob
import os
from os.path import dirname
import stat

HOMEPATH = os.path.expanduser("~")
DISTRIBUTION_BASE_DIR = dirname(dirname(dirname(os.path.realpath(__file__))))


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
