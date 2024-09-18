#!/bin/sh

set -e

./em_mergesort 268435456 0.01 5 1
./em_mergesort 268435456 0.1 5 1
./em_mergesort 268435456 0.3 5 1
./em_mergesort 268435456 0.5 5 1
