#!/bin/bash

EXECUTABLE=../build/src/approximation

REPORT_FILE=../output/approximation_report.csv

OUT_FOLDER=../output/Track1/approximation/

TIMEOUT_LIMIT=60

# TODO build csv report
echo "file, graph_nodes,graph_edges,graph_weight,terminals,parse_time,steiner_weight,steiner_time" > $REPORT_FILE

for FILENAME in ../input/Track1/*; do
    echo "Processing $FILENAME"

    OUTFILE="${OUT_FOLDER}$(basename $FILENAME)"
    echo "$OUTFILE"

    COMMAND="timeout ${TIMEOUT_LIMIT} ${EXECUTABLE} -gr ${FILENAME} -o ${OUTFILE} -t 1"
    OUT=$($COMMAND)

    GRAPH_NODES=$(echo "$OUT" | grep "graph_nodes" | awk -F ':' '{print $2}')
    GRAPH_EDGES=$(echo "$OUT" | grep "graph_edges" | awk -F ':' '{print $2}')
    GRAPH_WEIGHT=$(echo "$OUT" | grep "graph_weight" | awk -F ':' '{print $2}')
    TERMINALS=$(echo "$OUT" | grep "terminals" | awk -F ':' '{print $2}')
    PARSE_TIME=$(echo "$OUT" | grep "parse_time" | awk -F ':' '{print $2}')
    STEINER_WEIGHT=$(echo "$OUT" | grep "steiner_weight" | awk -F ':' '{print $2}')
    STEINER_TIME=$(echo "$OUT" | grep "steiner_time" | awk -F ':' '{print $2}')

    if [ -z "${GRAPH_NODES}" ]; then
        echo "Parsing error"
        GRAPH_NODES="-1"
        GRAPH_EDGES="-1"
        GRAPH_WEIGHT="-1"
        TERMINALS="-1"
        PARSE_TIME="-1"
    fi

    if [ -z "${STEINER_TIME}" ]; then
        echo "steiner timeout"
        STEINER_WEIGHT="-1"
        STEINER_TIME="-1"
    fi

    FILENAME_BASE=$(basename $FILENAME)
    echo "$FILENAME_BASE"

    echo "$FILENAME_BASE,$GRAPH_NODES,$GRAPH_EDGES,$GRAPH_WEIGHT,$TERMINALS,$PARSE_TIME,$STEINER_WEIGHT,$STEINER_TIME" >> $REPORT_FILE

    echo "sleep"
    sleep 1
done