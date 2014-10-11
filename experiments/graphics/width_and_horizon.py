"""

"""

import argparse
import matplotlib.pyplot as plt
import sys

sys.path.append('..')

from analyser import EXPERIMENTS_BASE_DIR, iterate
from graphics.lib.graphicsHelper import get_stylecycler, save_figure
from graphics.common import CSV_ROWS
from graphics.lib.utils import average_data, load_csv, filter_results, create_subplot
from src.helper import mkdirp, parse_filename, label_from_parameters


def parse_arguments():
    parser = argparse.ArgumentParser(description='Analyse the output of experiments.')
    parser.add_argument("--dir", required=True, help='The directory with the experiments\' output')
    parser.add_argument("--csv", action='store_true',
                        help='Whether to extract the csv data from the HDF5 file', default=False)
    args = parser.parse_args()
    return args


def load_data(experiment_dir, csv, csv_col):

    results = dict()

    for parameter_string, run, csv in iterate(experiment_dir, csv):
        params = parse_filename(parameter_string)
        hashable_params = frozenset(params.items())

        assert not hashable_params in results, "results already defined for parameters {}".format(hashable_params)

        data = load_csv_data(csv + '/agent-{}.csv'.format(params['agent']), csv_col)
        if data is not None:
            results[hashable_params] = data

    # Average the data:
    avg_data = average_data(results)

    return avg_data


def load_csv_data(csv_file, csv_col):
    res = load_csv(csv_file, usecols=(csv_col,))
    return res if res.size != 1 else None


def plot_data(data, output_dir):

    horizons = ['2', '4', '6', '8', '10', '12']
    horizons = ['6', '8', '10', '12']

    random_data = filter_results(data, agent='random', filter_keys=False)
    assert len(random_data) == 1
    random_values = list(random_data.values())[0]

    axes = create_subplot(2, 2)

    for index, h in enumerate(horizons):
        stylecycler = get_stylecycler()

        # axes[index].plot(range(0, random_values.size),
        #                  random_values, next(stylecycler), label=label_from_parameters({'agent': 'random'}))

        filtered = filter_results(data, horizon=h)
        params = sorted(filtered.keys(), key=lambda x: int(dict(x)['width']))
        for param in params:
            style = next(stylecycler)
            values = filtered[param]
            axes[index].plot(range(0, values.size), values, style, label=label_from_parameters(param))

            #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
            axes[index].legend(loc=2, fontsize=10, ncol=2)

            axes[index].set_title('horizon={}'.format(h), fontsize=12)
            axes[index].tick_params(axis='both', which='both', labelsize=8)  # Change the tick labels fontsize

            # axes[index].axis([100, 200, 200, 550])  # [xmin, xmax, ymin, ymax]

            # plt.ylabel('Resource units')
            # plt.xlabel('Time steps')

    axes[0].set_ylabel('Accumulated resource units', fontsize=12)
    axes[2].set_ylabel('Accumulated resource units', fontsize=12)
    axes[2].set_xlabel('Time', fontsize=12)
    axes[3].set_xlabel('Time', fontsize=12)

    save_figure(output_dir, 'resource-accumulation.c_3'.format())


def main():
    args = parse_arguments()
    data = load_data(EXPERIMENTS_BASE_DIR + '/' + args.dir, args.csv, CSV_ROWS['RESOURCES'])
    output_dir = './output'
    mkdirp(output_dir)
    plot_data(data, output_dir)

if __name__ == '__main__':
    main()