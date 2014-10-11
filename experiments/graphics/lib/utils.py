from collections import defaultdict
import os
import errno
import numpy
import re
import matplotlib.pyplot as plt
import sys



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


def average_data(results, average_params=['run']):
    aggregated = {}
    runs = defaultdict(int)

    for p, data in results.items():
        params = dict(p)  # Convert from frozen set back to a dictionary

        for avg_param in average_params:
            if not avg_param in params:
                raise RuntimeError("Failed to average on some parameter: {}".format(average_params))
            del params[avg_param]

        clean = frozenset(params.items())
        runs[clean] += 1
        aggregated[clean] = data if clean not in aggregated else aggregated[clean] + data

    check_runs(runs)

    return {params: d / runs[params] for params, d in aggregated.items()}


def check_runs(runs_per_width):
    runs = list(runs_per_width.values())
    assert len(runs) > 0
    if not all(runs[0] == item for item in runs):
        print("WARNING: NOT ALL WIDTHS HAVE THE SAME NUMBER OF RUNS. RUNS PER WIDTH:")
        for k, v in runs_per_width.items():
            print("{}: {}".format(k, v))


def filter_results(data, filter_keys=True, **kwargs):
    filtered = {}
    for k, v in data.items():
        params = dict(k)
        if all(key in params and params[key] == str(value) for key, value in kwargs.items()):
            if filter_keys:
                k = k.difference(frozenset(kwargs.items()))
            filtered[k] = v
    return filtered


def load_csv_data(csv_file, csv_col):
    res = load_csv(csv_file, usecols=(csv_col,))
    return res if res.size != 1 else None


def create_subplot(nrows, ncols, figsize=(8, 8), **kwargs):
    fig, axes = plt.subplots(nrows, ncols, figsize=figsize, dpi=600, **kwargs)
    plt.subplots_adjust(hspace=0.3)
    fig.tight_layout()
    return axes.ravel()