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


def make_filename(**kwargs):
    keys = sorted(kwargs.keys())
    assert not any('_' in k for k in keys), "Underscores not allowed in parameter key names"
    return '.'.join('{}_{}'.format(k, kwargs[k]) for k in keys)


def parse_filename(name):

    # Temporary hack
    if 'random_run' in name:
        return {'agent': 'random', 'run': int(name[11:])}

    parameters = name.split('.')
    d = {}
    for param in parameters:
        key_value = param.split('_')
        assert len(key_value) == 2, "Wrong filename format: {}".format(param)
        d[key_value[0]] = key_value[1]
    return d


def label_from_parameters(parameters):
    d = dict(parameters)
    if d['agent'] == 'random':
        return 'random'
    elif d['agent'] == 'mdp':
        return 'mdp<h={},w={}>'.format(d['horizon'], d['width'])
    else:
        assert False, "TO DO"

