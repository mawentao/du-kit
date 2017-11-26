#!/bin/bash

####################################################
# @file:   build.sh
# @author: _AUTHOR_
# @create: _DATETIME_
# @modify: _DATETIME_
# @brief:  build.sh
####################################################

version="1.0.0"
appname="_FILENAME_"
builddate=`date +%s`
tarname="$appname-$version.$builddate.tar.gz"
outdir="./output/$appname"

function cpfiles()
{
    for i in $@; do
        cp -r $i $outdir
    done
}

rm -rf output
mkdir -m output
mkdir -p $outdir/conf
cp -r conf/online/* $outdir/conf
cpfiles models mt run.php scripts

cd output; find . -type d -name ".svn" | xargs rm -rf
tar zcvf $tarname $appname/*

exit 0
