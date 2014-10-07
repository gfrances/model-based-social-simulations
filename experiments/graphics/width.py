"""

"""

import argparse
from collections import defaultdict
import matplotlib.pyplot as plt
import sys

sys.path.append('..')

from analyser import EXPERIMENTS_BASE_DIR, iterate
from graphics.lib import utils
from graphics.lib.graphicsHelper import get_stylecycler, save_figure
from src.helper import mkdirp


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


def load_data(experiment_dir, csv):

    results = dict()
    runs_per_width = defaultdict(int)

    for width, run, csv in iterate(experiment_dir, csv):
        w = int(width[6:])
        r = int(run[4:])
        print(width, w, run, r, csv)

        data = load_csv_data(csv + '/agent-mdp.csv')

        if data is not None:
            results[w] = data if w not in results else results[w] + data
            runs_per_width[w] += 1

    check_runs(runs_per_width)

    # Average the data:
    avg_data = {width: d / runs_per_width[width] for width, d in results.items()}
    return avg_data


def load_csv_data(csv_file):
    res = utils.load_csv(csv_file, usecols=(1,))
    return res if res.size != 1 else None


def plot_data(data, output_dir):
    stylecycler = get_stylecycler()

    widths = sorted(data.keys())
    xmax, ymax = 0, 0
    for width in widths:
        style = next(stylecycler)
        values = data[width]
        plt.plot(range(0, values.size), values, style, label='w={}'.format(width))
        xmax = max(xmax, values.size)
        ymax = max(ymax, max(values))

    #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
    plt.legend(loc=2)
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