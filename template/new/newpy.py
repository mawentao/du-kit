#!/usr/bin/env python
#coding=utf-8

####################################################
# @file:   _FILENAME_.py
# @author: _AUTHOR_
# @create: _DATETIME_
# @modify: _DATETIME_
# @brief:  _FILENAME_.py
####################################################

import os
import sys

def print_usage():
    pname = sys.argv[0]
    print "[usage] %s filename" % (pname)
    exit(-1)

def read_file(f):
    try :
        fi = open(f, "r")
        line = fi.readline()
        while line:
            print line.strip()
            line = fi.readline()
    except Exception as e :
        sys.stderr.write(str(e))


if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)
    if (argc<2):
        print_usage()
    read_file(argv[1])

    print '!!!THE END!!!.'
