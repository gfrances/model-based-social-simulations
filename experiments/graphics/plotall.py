#
# Plot the results of all the desired experiments.
#
from lib.common import setup_matplotlib
import population
import singleagent
import width

OUTPUT_DIR = 'output'


def main():
    setup_matplotlib()
    # singleagent.plot(OUTPUT_DIR)
    # width.plot(OUTPUT_DIR)
    population.plot(OUTPUT_DIR)


if __name__ == '__main__':
    main()