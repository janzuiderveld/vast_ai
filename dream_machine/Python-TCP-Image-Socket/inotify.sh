#!/bin/sh
touch Sketch-Simulator/out.log
MONITORDIR="Sketch-Simulator/out/to_send"
inotifywait -m -r -e create --format '%w%f' "${MONITORDIR}" | while read NEWFILE
do
        echo "${NEWFILE}" >> out.log
done