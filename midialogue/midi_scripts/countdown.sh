#/usr/bin/bash

# This script is used to count down from the specified number of seconds.
# It saves the time in a file called countdown.txt

# The script is called by the following command:
# ./countdown.sh 10 cmd

count=$1
command=$2
command2=$3

# get the current location of the script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR

echo $count > $DIR/countdown.txt

while [ $count -gt 0 ] ; do
    count=$(cat $DIR/countdown.txt)
        
    echo $count

    sleep 1

    count=$(cat $DIR/countdown.txt)
    count=$(( $count - 1 ))

    echo $count > $DIR/countdown.txt

done

# run the command
$command
echo f"executed command: $command"

# run the second command
$command2
echo f"executed command: $command2"

exit 0