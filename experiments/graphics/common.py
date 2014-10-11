"""

"""

CSV_ROWS = {
    'POPULATION': 1,
    'RESOURCES': 2,
}

AGENT_ORDER = ['motionless', 'random', 'lazy', 'greedy', 'mdp']

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



