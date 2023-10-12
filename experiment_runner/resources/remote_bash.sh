#!/bin/bash

# Redirect all nodes serial output to a file
readonly OUTFILE="${HOME}/.iot-lab/${EXP_ID}/serial_output"

echo "Launch serial_aggregator with exp_id==${EXP_ID}" >&2
serial_aggregator -i ${EXP_ID} 2> /dev/null 1> ${OUTFILE}
