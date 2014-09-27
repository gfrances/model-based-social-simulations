#
# Plot the data regarding the experiments that try to analyze the impact
# of different decision-making strategies on a single-agent simulation.
#

import matplotlib.pyplot as plt
from lib import utils
from lib.graphicsHelper import save_figure, get_stylecycler


EXPLORED_WIDTHS = [500]

DATASETS = {
    'random-mdp': 'RandomMDP_agents100_years10_climate4000.0_2000.0_mb0.10_binWOK_{run_name}.csv',
    'rule-based': 'DecisionTree_alpha0.5_agents100_years10_climate4000.0_2000.0_mb0.10_binWOK_{run_name}.csv',
    # 'mdp_h3': 'MDP_h3_w{width}_agents1_years1_climate2000.0_1.0_mb0.10_binWOK_{run_name}.csv',
    #         'MDP_h6_w500_agents100_years10_climate4000.0_1200.0_mb0.10_binWOK_exec0.csv'
    'mdp_h6': 'MDP_h6_w{width}_agents100_years10_climate4000.0_2000.0_mb0.10_binWOK_{run_name}.csv',
    # 'mdp_h10': 'MDP_h10_w{width}_agents1_years1_climate2000.0_1.0_mb0.10_binWOK_{run_name}.csv'
}


def get_dataset_name(agent, width, run_name):
    return DATASETS[agent].format(run_name=run_name, width=width)


def get_dataset_label(agent):
    return {
        'rule-based': 'Rule-based',
        'random-mdp': "Random",
        'mdp_h3': 'MDP(h=3)',
        'mdp_h6': 'MDP(h=6)',
        'mdp_h10': 'MDP(h=10)',
    }[agent]


ORDER = ['rule-based', 'random-mdp', 'mdp_h3', 'mdp_h6', 'mdp_h10']
ORDER = ['rule-based', 'random-mdp', 'mdp_h6']

def load_run_data(width, run_name):
    data = {}
    for agent in DATASETS.keys():
        filename = get_dataset_name(agent, width, run_name)
        intermediate = run_name + '/climate2000.0_1.0/'
        intermediate = ''
        data[agent] = utils.load_csv('./data/multiagent/' + intermediate + filename)
    return data


def non_decreasing(L):
    return all(x <= y for x, y in zip(L, L[1:]))


def check_non_decreasing(l):
    assert(non_decreasing(l[:-2]))


def accum_data(width, runs):

    accum = {}
    for r in runs:
        run_data = load_run_data(width, "exec{0}".format(r))

        for k, d in run_data.items():
            # check_non_decreasing(list(d[:, 1]))
            accum[k] = accum[k] + d if k in accum else d

    return accum


def load_data(width):
    runs = range(0, 10)
    runs = [0, 2, 3, 4, 5, 6, 7, 8, 9]

    data = accum_data(width, runs)
    avg_data = {k: d.copy() / len(runs) for k, d in data.items()}  # Average the data over the different runs

    return avg_data


def plot_data(data, width, output_dir):
    stylecycler = get_stylecycler()

    for agent, values in ((k, data[k]) for k in ORDER):  # Iterate in the desired order.
        style = next(stylecycler)
        print(agent, style)
        plt.plot(values[:, 0], values[:, 1], style, label=get_dataset_label(agent))

    #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
    plt.legend(loc=2)
    plt.axis([0, 3590, 0, 1000])  # [xmin, xmax, ymin, ymax]
    #plt.xticks(sizes, sizes, size='small')
    plt.ylabel('(Accumulated) starvation rate')
    plt.xlabel('Time steps')

    save_figure(output_dir, 'avg_starvation_rate_w{}'.format(width))


def plot(output_dir):
    for width in EXPLORED_WIDTHS:
        data = load_data(width)
        plot_data(data, width, output_dir)
