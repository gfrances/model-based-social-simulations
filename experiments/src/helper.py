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
    assert not any('_' in k for k in keys) and not any(isinstance(k, str) and '_' in k for k in kwargs.values()),\
        "Underscores not allowed in parameter key names"
    return '.'.join('{}_{}'.format(k, kwargs[k]) for k in keys)


def parse_filename(name):
    parameters = name.split('.')
    d = {}
    for param in parameters:
        key_value = param.split('_')
        assert len(key_value) == 2, "Wrong filename format: {}".format(param)
        d[key_value[0]] = key_value[1]
    return d


def label_from_parameters(parameters):
    d = dict(parameters)
    if d['agent'] in ['random', 'motionless', 'lazy', 'greedy']:
        return d['agent']
    elif d['agent'] == 'mdp':
        if 'horizon' in d and 'width' in d:
            return 'mdp[h={},w={}]'.format(d['horizon'], d['width'])
        elif 'horizon' in d:
            return 'mdp[h={}]'.format(d['horizon'])
        elif 'width' in d:
            # return 'mdp[w={}]'.format(d['width'])
            return 'w={}'.format(d['width'])
        else:
            return 'mdp'
    else:
        raise RuntimeError("Unknown agent type: {}".format(d['agent']))

