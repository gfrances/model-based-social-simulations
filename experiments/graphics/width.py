"""

"""

import argparse
from collections import defaultdict
from matplotlib.font_manager import FontProperties
import matplotlib.pyplot as plt
import sys

sys.path.append('..')

from analyser import EXPERIMENTS_BASE_DIR, iterate
from graphics.lib import utils
from graphics.lib.graphicsHelper import get_stylecycler, save_figure
from src.helper import mkdirp, parse_filename, label_from_parameters


AGENT_TYPES = {
    'rule-based': 'Rule-based',
    'random': "Random",
    'lazy': "Lazy",
    'mdp': 'MDP',
    'greedy': 'Greedy',
    'motionless': 'Null',
}


def get_agent_label(agent_type):
    return AGENT_TYPES[agent_type.lower()]


def parse_arguments():
    parser = argparse.ArgumentParser(description='Analyse the output of experiments.')
    parser.add_argument("--dir", required=True, help='The directory with the experiments\' output')
    parser.add_argument("--csv", action='store_true',
                        help='Whether to extract the csv data from the HDF5 file', default=False)
    args = parser.parse_args()
    return args


def check_runs(runs_per_width):
    runs = list(runs_per_width.values())
    assert len(runs) > 0
    if not all(runs[0] == item for item in runs):
        print("WARNING: NOT ALL WIDTHS HAVE THE SAME NUMBER OF RUNS. RUNS PER WIDTH: {}".format(runs_per_width))


def average_data(results):
    aggregated = {}
    runs = defaultdict(int)

    for p, data in results.items():
        params = dict(p)  # Convert from frozen set back to a dictionary
        assert 'run' in params, "The experiment configuration object needs to have a run ID"
        del params['run']
        clean = frozenset(params.items())
        runs[clean] += 1
        aggregated[clean] = data if clean not in aggregated else aggregated[clean] + data

    check_runs(runs)

    return {params: d / runs[params] for params, d in aggregated.items()}


def load_data(experiment_dir, csv):

    results = dict()

    for parameter_string, run, csv in iterate(experiment_dir, csv):
        params = parse_filename(parameter_string)
        hashable_params = frozenset(params.items())

        assert not hashable_params in results, "results already defined for parameters {}".format(hashable_params)

        data = load_csv_data(csv + '/agent-{}.csv'.format(params['agent']))
        if data is not None:
            results[hashable_params] = data

    # Average the data:
    avg_data = average_data(results)

    return avg_data


def load_csv_data(csv_file):
    res = utils.load_csv(csv_file, usecols=(1,))
    return res if res.size != 1 else None


def plot_data(data, output_dir):
    stylecycler = get_stylecycler()

    params = sorted(data.keys())
    xmax, ymax = 0, 0
    for param in params:
        style = next(stylecycler)
        values = data[param]
        label = label_from_parameters(param)
        plt.plot(range(0, values.size), values, style, label=label)
        xmax = max(xmax, values.size)
        ymax = max(ymax, max(values))

    font = FontProperties()
    font.set_size(8)
    # legend([plot1], "title", prop = fontP)

    #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
    plt.legend(loc=2, prop=font)
    plt.axis([0, xmax + 10, 0, ymax + 10])  # [xmin, xmax, ymin, ymax]
    #plt.xticks(sizes, sizes, size='small')
    plt.ylabel('Resource units')
    plt.xlabel('Time steps')

    save_figure(output_dir, 'population-dynamics{}'.format(''))


def main():
    args = parse_arguments()
    data = load_data(EXPERIMENTS_BASE_DIR + '/' + args.dir, args.csv)
    output_dir = './output'
    mkdirp(output_dir)
    plot_data(data, output_dir)

if __name__ == '__main__':
    main()