#!/bin/bash

set -e

for ((i=1;i<=${nlines};i++)); do
    ${runtask} $$i
done