#!/bin/bash

SIMULPAST_DIR=${simulpast_dir}

case $${SGE_TASK_ID} in

${task_options}
*) echo error in SGE_TASK_ID:$${SGE_TASK_ID} ;;

esac