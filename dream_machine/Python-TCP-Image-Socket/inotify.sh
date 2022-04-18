#!/bin/sh
touch Sketch-Simulator/out.log
MONITORDIR="8081_images0"
inotifywait -m -r -e create --format '%w%f' "${MONITORDIR}" | while read NEWFILE
do
        echo "${NEWFILE}" >> out.log
done