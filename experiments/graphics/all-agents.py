"""

"""

import argparse
import sys
from graphics.common import AGENT_ORDER
from graphics.lib.utils import average_data, filter_results, load_csv_data, create_subplot
from graphics.width_and_horizon import CSV_ROWS

sys.path.append('..')

from analyser import EXPERIMENTS_BASE_DIR, iterate
from graphics.lib.graphicsHelper import get_stylecycler, save_figure
from src.helper import mkdirp, parse_filename, label_from_parameters


def parse_arguments():
    parser = argparse.ArgumentParser(description='Analyse the output of experiments.')
    parser.add_argument("--dir", required=True, help='The directory with the experiments\' output')
    parser.add_argument("--csv", action='store_true',
                        help='Whether to extract the csv data from the HDF5 file', default=False)
    args = parser.parse_args()
    return args


def load_data(experiment_dir, csv, csv_col, restrict=None):
    """

    :param experiment_dir:
    :param csv:
    :param csv_col:
    :param restrict:
    :return:
    """

    results = dict()

    for parameter_string, run, csv in iterate(experiment_dir, csv):
        params = parse_filename(parameter_string)
        hashable_params = frozenset(params.items())

        # An extremely simple mechanism to restrict the exploration to a single parameter value.
        if restrict:
            assert isinstance(restrict, tuple) and len(restrict) == 2
            k, v = restrict  # unpack values
            if params[k] != v:
                continue

        assert not hashable_params in results, "results already defined for parameters {}".format(hashable_params)

        data = load_csv_data(csv + '/agent-{}.csv'.format(params['agent']), csv_col)
        if data is not None:
            results[hashable_params] = data

    # Average the data:
    avg_data = average_data(results, ['run', 'map'])

    return avg_data


def plot_lambda_analysis(data, output_dir, autocorrelation):
    axes = create_subplot(2, 2, figsize=(9, 6))

    consumptions = ['2', '3', '4', '5']

    for index, consumption in enumerate(consumptions):
        stylecycler = get_stylecycler()

        # Filter by the parameter that we focus on this subplot
        filtered = filter_results(data, consumption=consumption, autocorrelation=autocorrelation)

        # Sort according to the position in the order vector
        params = sorted(filtered.keys(), key=lambda x: AGENT_ORDER.index(dict(x)['agent']))

        legend_locs = [4, 1, 2, 1]  # We need a different location for each legend!
        ncols = [2, 1, 1, 1]

        for param in params:
            values = filtered[param]
            axes[index].plot(range(0, values.size), values, next(stylecycler),
                             linewidth=2,  # Increase the linewidth for this graphic, which will be displayed very small
                             label=label_from_parameters(param))

            #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
            axes[index].legend(loc=legend_locs[index], fontsize=10, ncol=ncols[index])

            axes[index].set_title(r'$\lambda$={}'.format(consumption), fontsize=12)

            axes[index].tick_params(axis='both', which='both', labelsize=8)  # Change the tick labels fontsize

            # axes[index].axis([0, 200, 0, 1000])  # [xmin, xmax, ymin, ymax]

    # axes[0].set_xlim([0, 200])

    axes[0].set_ylabel('Population', fontsize=12)
    axes[2].set_ylabel('Population', fontsize=12)
    axes[2].set_xlabel('Time', fontsize=12)
    axes[3].set_xlabel('Time', fontsize=12)

    save_figure(output_dir, 'population-dynamics.c_{}'.format(autocorrelation))


def plot_consumption_analysis(data, output_dir, consumption):
    axes = create_subplot(1, 3, figsize=(12, 4))

    autocorrelations = ['1', '10', '25']

    for index, autocorrelation in enumerate(autocorrelations):
        stylecycler = get_stylecycler()

        # Filter by the parameter that we focus on this subplot
        filtered = filter_results(data, consumption=consumption, autocorrelation=autocorrelation)

        # Sort according to the position in the order vector
        params = sorted(filtered.keys(), key=lambda x: AGENT_ORDER.index(dict(x)['agent']))

        for param in params:
            values = filtered[param]
            axes[index].plot(range(0, values.size), values, next(stylecycler),
                             linewidth=2,  # Increase the linewidth for this graphic, which will be displayed very small
                             label=label_from_parameters(param))

            #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
            # axes[index].legend(bbox_to_anchor=(1, 0.9), fontsize=8, ncol=2)
            axes[index].legend(loc=2, fontsize=10, ncol=1)
            axes[index].set_title(r'autocorrelation={}'.format(autocorrelation), fontsize=12)
            axes[index].tick_params(axis='both', which='both', labelsize=8)  # Change the tick labels fontsize

            axes[index].axis([0, 1000, 0, 350])  # [xmin, xmax, ymin, ymax]

    axes[0].set_ylabel('Population', fontsize=12)
    for axe in axes:
        axe.set_xlabel('Time', fontsize=12)

    save_figure(output_dir, 'population-dynamics.lambda_{}'.format(consumption))


def main():
    args = parse_arguments()

    exp_dir = EXPERIMENTS_BASE_DIR + '/' + args.dir
    data = load_data(exp_dir, args.csv, CSV_ROWS['POPULATION'])
    output_dir = './output'
    mkdirp(output_dir)
    plot_lambda_analysis(data, output_dir, autocorrelation='25')
    plot_consumption_analysis(data, output_dir, consumption='3')

if __name__ == '__main__':
    main()