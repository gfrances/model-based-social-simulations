
import os
import errno
import numpy
import re


def mkdir_p(path):
    """ 'mkdir -p' like functionality"""
    try:
        os.makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


def capitalize(s):
    return s[0].upper() + s[1:]


def slugify(value):
    """
    Normalizes string, converts to lowercase, removes non-alpha characters,
    and converts spaces to hyphens.
    """
    value = re.sub('[^\w\s-]', '', value).strip().lower()
    return re.sub('[-\s]+', '-', value)


def camelcaseToSlug(name):
    """
    http://stackoverflow.com/questions/1175208/elegant-python-function-to-convert-camelcase-to-camel-case
    """
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1-\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1-\2', s1).lower()


def load_csv(filename, delimiter=';', **kwargs):
    print("Loading csv file at {0}".format(filename))
    return numpy.loadtxt(filename, delimiter=delimiter, skiprows=1, **kwargs)