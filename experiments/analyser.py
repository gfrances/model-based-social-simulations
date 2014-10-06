"""
    A small utility file to extract CSV data from HDF5 files and iterate over the results
"""

import argparse
import os
import subprocess
import sys
from src.helper import get_subdirs, mkdirp

HOMEPATH = os.path.expanduser("~")
EXPERIMENTS_BASE_DIR = HOMEPATH + '/projects/simulpast/experiments'
ANALYSIS_BIN = HOMEPATH + '/projects/simulpast/model-based-social-simulations/analysis/analysis.bin'


def iterate(experiment_dir, csv):
    for subdir, last in get_subdirs(experiment_dir):
        if not os.path.isdir(subdir) or last == "run":
            continue

        for rundir, run in get_subdirs(subdir, "run_*"):
            print("Analysing data on directory {}".format(rundir))
            csv_dir = rundir + '/csv'
            if csv:
                generate_csv(rundir + '/output/model.h5', csv_dir)
            yield last, run, csv_dir


def generate_csv(file, output_dir):
    mkdirp(output_dir)  # Just in case

    command = "{} {} {}".format(ANALYSIS_BIN, file, output_dir)
    sys.stdout.flush()  # Flush the output to avoid it mixing with the subprocess call.
    output = subprocess.call(command.split())
    if not output == 0:
        raise RuntimeError('Error executing command "{0}"'.format(command))


def parse_arguments():
    parser = argparse.ArgumentParser(description='Analyse the output of experiments.')
    parser.add_argument("--dir", required=True, help='The directory with the experiments\' output')
    parser.add_argument("--csv", action='store_true',
                        help='Whether to extract the csv data from the HDF5 file', default=False)
    args = parser.parse_args()
    return args


def main():
    args = parse_arguments()

    experiment_dir = EXPERIMENTS_BASE_DIR + '/' + args.dir

    for width, run, csv in iterate(experiment_dir, args.csv):
        print(width, run, csv)


if __name__ == '__main__':
    main()