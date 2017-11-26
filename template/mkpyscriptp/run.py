#!/usr/bin/env python
#coding=utf-8

####################################################
# @file:   main.py
# @author: _AUTHOR_
# @create: _DATETIME_
# @modify: _DATETIME_
# @brief:  脚本运行入口
####################################################

import os
import sys
sys.path.append(".")
from mtlib import *
import traceback

def print_usage():
    global sys
    help = "usage: python %s <ScriptClassName> argv" % (sys.argv[0])
    print help
    exit(0)

def parse_args():
    global sys
    argc = len(sys.argv)
    if (argc<2):
        print_usage()
    args = sys.argv[2:]
    for arg in sys.argv:
        if (arg=="-h" or arg=="--help"):
            print_usage()
    return args

def set_dbo(conf, key):
    host = conf.get(key, "host")
    port = conf.getint(key, "port")
    user = conf.get(key, "user")
    word = conf.get(key, "pass")
    db   = conf.get(key, "db")
    dbo  = DBO(host, port, user, word, db)
    Env.setDBO(dbo, key)


if __name__=='__main__':
    args = parse_args()
    ScriptName = sys.argv[1]
    try:
        Env.init("./conf", "main")
        conf = Env.getconf()
        #set_dbo(conf, "mysql")
        scripts = __import__("scripts")    
        s = getattr(scripts, ScriptName)()
        Env.getlog().trace("start script ("+ScriptName+")");
        s.run(args)
        Env.getlog().trace("finish script ("+ScriptName+")");
    except Exception as e:
        if Env.isDebug():
            traceback.print_exc()
        else:
            print "[Error]: can not find script ( %s ) or execute fail!" % (ScriptName)

