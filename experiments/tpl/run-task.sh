#!/bin/bash

SIMULPAST_DIR=${simulpast_dir}

if [[ -d $${HOME}/projects/simulpast/pandora-main ]]; then
    # Export both the location of the PANDORA library and that of the local libs, possibly including boost.
    export LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:$${HOME}/projects/simulpast/pandora-main/lib:$${HOME}/local/lib
else
    echo "Cannot locate the PANDORA library path"
    exit 1
fi

case $${SGE_TASK_ID} in

${task_options}
*) echo error in SGE_TASK_ID:$${SGE_TASK_ID} ;;

esac