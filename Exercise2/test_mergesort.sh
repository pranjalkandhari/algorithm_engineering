#!/bin/sh

set -e

./build/MergeSort -t 1000 12
./build/OMPMergeSort -t 1000 12 5
