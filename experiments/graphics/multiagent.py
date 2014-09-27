from itertools import cycle
import matplotlib.pyplot as plt
from lib import utils
from lib.common import setup_matplotlib
from lib.graphicsHelper import save_figure, LINE_PALETTE, CONTRAST_PALETTE


DATASETS = {
    'rule-based': 'DecisionTree_alpha0.5_agents1_years5_climate2000.0_1.0_mb0.10_binWOK_{run_name}.csv',
    'mdp_h3': 'MDP_h3_w{width}_agents1_years1_climate2000.0_1.0_mb0.10_binWOK_{run_name}.csv',
    'mdp_h6': 'MDP_h6_w{width}_agents1_years1_climate2000.0_1.0_mb0.10_binWOK_{run_name}.csv',
    'mdp_h10': 'MDP_h10_w{width}_agents1_years1_climate2000.0_1.0_mb0.10_binWOK_{run_name}.csv'
}


def get_dataset_name(agent, run_name):
    width = 1000
    return DATASETS[agent].format(run_name=run_name, width=width)


def get_dataset_label(agent):
    return {
        'rule-based': 'Rule-based',
        'mdp_h3': 'MDP(h=3)',
        'mdp_h6': 'MDP(h=6)',
        'mdp_h10': 'MDP(h=10)',
    }[agent]


ORDER = ['rule-based', 'mdp_h3', 'mdp_h6', 'mdp_h10']


def load_run_data(run_name):
    data = {}
    for agent in DATASETS.keys():
        filename = get_dataset_name(agent, run_name)
        data[agent] = utils.load_csv('./data/' + run_name + '/climate2000.0_1.0/' + filename)
    return data


def accum_data(runs):

    accum = {}
    for r in runs:
        run_data = load_run_data("exec{0}".format(r))

        for k, d in run_data.items():
            accum[k] = accum[k] + d if k in accum else d

    return accum


def load_data():
    runs = range(0, 10)

    data = accum_data(runs)
    avg_data = {k: d.copy() / len(runs) for k, d in data.items()}  # Average the data over the different runs

    return avg_data


def plot_data(data):
    stylecycler = cycle(('{}{}'.format(color, line) for color, line in zip(CONTRAST_PALETTE, LINE_PALETTE)))

    for agent, values in ((k, data[k]) for k in ORDER):  # Iterate in the desired order.
        style = next(stylecycler)
        plt.plot(values[:, 0], values[:, 1], style, label=get_dataset_label(agent))

    #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
    plt.legend(loc=2)
    plt.axis([0, 359, 0, 1000])  # [xmin, xmax, ymin, ymax]
    #plt.xticks(sizes, sizes, size='small')
    plt.ylabel('(Accumulated) starvation rate')
    plt.xlabel('Time steps')

    plt.show()
    save_figure('test')


def plot_multiagent_data():
    setup_matplotlib()
    data = load_data()
    plot_data(data)

if __name__ == '__main__':
    plot_multiagent_data()