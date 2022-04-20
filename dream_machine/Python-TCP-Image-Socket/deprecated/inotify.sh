#!/bin/sh
touch Sketch-Simulator/out.log
MONITORDIR="/workspace/vast_ai/dream_machine/Sketch-Simulator/out/to_send"
inotifywait -m -r -e create --format '%w%f' "${MONITORDIR}" | while read NEWFILE
do
        echo "${NEWFILE}" >> Sketch-Simulator/out.log
done