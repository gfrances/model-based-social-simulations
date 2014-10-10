#
# Plot the results of all the desired experiments.
#
from lib.common import setup_matplotlib
import population

OUTPUT_DIR = 'output'


def main():
    setup_matplotlib()
    population.plot(OUTPUT_DIR)


if __name__ == '__main__':
    main()