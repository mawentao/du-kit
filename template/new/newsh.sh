#!/bin/bash

####################################################
# @file:   _FILENAME_.sh
# @author: _AUTHOR_
# @create: _DATETIME_
# @modify: _DATETIME_
# @brief:  _FILENAME_.sh
####################################################

timestamp=`date +%s`
date=`date "+%Y-%m-%d %H:%M:%S"`


function read_file()
{
    if [ ! -f $1 ]; then
        echo "$1 is not file";
        return;
    fi
    cat $1 | while read line
	do
		echo $line
    done
}

if [ "$1"x = ""x ] ; then
    echo "usage: $0 file";
    exit
fi
read_file $1

echo '!!!THE END!!!'

exit 0
