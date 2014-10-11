#!/bin/bash

SIMULPAST_DIR=${HOME}/projects/simulpast
SIMULPAST_BIN=${SIMULPAST_DIR}/model-based-social-simulations/src/simulation.bin

if [[ -d ${HOME}/projects/simulpast/pandora-main ]]; then
    # Export both the location of the PANDORA library and that of the local libs, possibly including boost.
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/projects/simulpast/pandora-main/lib:${HOME}/local/lib
else
    echo "Cannot locate the PANDORA library path"
    exit 1
fi

TASK=${1:-$SGE_TASK_ID}
case ${TASK} in
1) python3 ${SIMULPAST_DIR}/model-based-social-simulations/experiments/analyser.py --csv --dir test_all.100sec.8GB.20141010_0227 ;;
*) echo error in SGE_TASK_ID:${TASK} ;;

esac
