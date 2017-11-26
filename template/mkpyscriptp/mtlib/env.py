#!/usr/bin/env python
#coding=utf-8

import os
import sys
sys.path.append(os.path.dirname(__file__)+"/mtcore/")
import time
import ConfigParser
from ublog import *

class Env:
    debugMode = False

    logConfDict = {
        "logpath" : "./log",
        "logname" : "main",
        "loglevel": LOG_DEBUG,
        "logslice": SLICE_NONE
    }
    loggers = {}

    CONF_PATH = "./conf"
    CONF_NAME = "main"
    confs = {}

    dbos = {}

    # ��̬��������ʼ��������:
    #   ��ȡ���ã��������ó�ʼ����־��debugģʽ
    @staticmethod
    def init(confpath="./conf", confname="main"):
        #1. ��ȡ����
        Env.CONF_PATH = confpath
        Env.CONF_NAME = confname
        conf = Env.getconf()

        #2. ��ʼ����־
        if conf.has_option("log", "log_name") : Env.logConfDict["logname"] = conf.get("log", "log_name")
        if conf.has_option("log", "log_path") : Env.logConfDict["logpath"] = conf.get("log", "log_path")
        if conf.has_option("log", "log_level") : Env.logConfDict["loglevel"] = conf.get("log", "log_level")
        if conf.has_option("log", "log_slice") : Env.logConfDict["logslice"] = conf.getint("log", "log_slice")
        logname = Env.logConfDict["logname"]
        Env.loggers[logname] = UBLog(Env.logConfDict)

        #3. ����debugģʽ
        Env.debugMode = conf.getboolean("global", "debug")

    # ��̬��������ȡ����ģʽ��־��
    @staticmethod
    def isDebug(): return Env.debugMode

    # ��̬��������ȡ��־ģ�飩
    @staticmethod
    def getlog(logname= None):
        if not logname:
            logname = Env.logConfDict["logname"]
        if not Env.logConfDict.has_key(logname):
            logdict = Env.logConfDict
            oldname = logdict["logname"]
            logdict["logname"] = logname
            Env.loggers[logname] = UBLog(logdict)
            logdict["logname"] = oldname
        return Env.loggers[logname];

    # ��̬��������ȡ�����ļ���
    @staticmethod
    def getconf(confname=None):
        if not confname:
            confname = Env.CONF_NAME
        if not Env.confs.has_key(confname):
            conffile = Env.CONF_PATH+"/"+confname+".ini"
            conf = ConfigParser.ConfigParser()
            if not os.path.isfile(conffile):
                sys.stderr.write("[ERROR] file does not exist [%s].\n" %conffile)
                exit(-1)
            conf.read(conffile)
            Env.confs[confname] = conf
        return Env.confs[confname]

    # ��̬��������ȡDBO��
    @staticmethod
    def getDBO(key="mysql"):
        if Env.dbos.has_key(key): return Env.dbos[key]
        else: return None

    # ��̬���� ������DBO��
    @staticmethod
    def setDBO(dbo, key="mysql"):
        if Env.dbos.has_key(key): return
        Env.dbos[key] = dbo
        

if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)


    print Env.getTime()

    print '!!!THE END!!!.'
