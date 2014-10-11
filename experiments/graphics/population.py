#
# Plot the population dynamics data.
#

import matplotlib.pyplot as plt
from lib import utils
from lib.graphicsHelper import save_figure, get_stylecycler


DATASETS = {
    'random': '',
    'lazy': '',
    'mdp': '',
    'motionless': '',
    'greedy': '',
}


def get_dataset_name(agent, run_name):
    return "agent-{}.csv".format(agent)


def get_dataset_label(agent):
    return {
        'rule-based': 'Rule-based',
        'random': "Random",
        'lazy': "Lazy",
        'mdp': 'MDP',
        'greedy': 'Greedy',
        'motionless': 'Null',
    }[agent]


ORDER = ['mdp', 'random', 'lazy', 'greedy', 'motionless']
ORDER = ['mdp', 'random', "lazy", "greedy"]


def load_run_data(run_name):
    base_dir = '../../analysis/'
    data = {}
    for agent in ORDER:
        filename = get_dataset_name(agent, run_name)
        data[agent] = utils.load_csv(base_dir + filename, usecols=(0, 1))
    return data


def non_decreasing(L):
    return all(x <= y for x, y in zip(L, L[1:]))


def check_non_decreasing(l):
    assert(non_decreasing(l[:-2]))


def accum_data(runs):

    accum = {}
    for r in runs:
        run_name = "run={0}".format(r)
        run_name = ''
        run_data = load_run_data(run_name)

        for k, d in run_data.items():
            accum[k] = accum[k] + d if k in accum else d

    return accum


def load_data():
    runs = range(0, 10)
    runs = [0]  # 1 single run for the moment

    data = accum_data(runs)
    avg_data = {k: d.copy() / len(runs) for k, d in data.items()}  # Average the data over the different runs

    return avg_data


def plot_data(data, output_dir):
    stylecycler = get_stylecycler()

    for agent, values in ((k, data[k]) for k in ORDER):  # Iterate in the desired order.
        style = next(stylecycler)
        print(agent, style)
        plt.plot(values[:, 0], values[:, 1], style, label=get_dataset_label(agent))

    #plt.legend(loc=2,bbox_to_anchor=(0., 1.02, 1., .102))
    plt.legend(loc=2)
    plt.axis([0, 200, 0, 50])  # [xmin, xmax, ymin, ymax]
    #plt.xticks(sizes, sizes, size='small')
    plt.ylabel('Population')
    plt.xlabel('Time steps')

    save_figure(output_dir, 'population-{}'.format(''))


def plot(output_dir):
    data = load_data()
    plot_data(data, output_dir)

if __name__ == '__main__':
    plot('output')