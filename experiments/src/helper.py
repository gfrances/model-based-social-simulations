#
# Some helper methods
#
import os
import stat

HOMEPATH = os.path.expanduser("~")


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
