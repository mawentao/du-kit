#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   UBLog.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2014-11-28 13:43:04
# @modify: 2014-11-28 13:43:04
# @brief:  日志模块
####################################################

import os
import re
import sys
import time
import logging
import logging.handlers


# 日志级别
LOG_FATAL   = 0
LOG_WARNING = 2
LOG_NOTICE  = 4
LOG_TRACE   = 8
LOG_DEBUG   = 16

# 日志文件切割方式
SLICE_NONE  = 0
SLICE_MONTH = 1
SLICE_DAY   = 2
SLICE_HOUR  = 3


class UBLog:

    def __init__(self, confDict):
        self.__loggers = {}

        self.__logname = confDict["logname"] if confDict.has_key("logname") else "main"
        self.__logpath = confDict["logpath"] if confDict.has_key("logpath") else "./log"
        self.__loglevel= confDict["loglevel"] if confDict.has_key("loglevel") else LOG_DEBUG
        self.__logslice= confDict["logslice"] if confDict.has_key("logslice") else SLICE_NONE
        if not os.path.isdir(self.__logpath):
            os.makedirs(self.__logpath)


    def debug(self, log):
        self.write_log(LOG_DEBUG, log)
    def trace(self, log):
        self.write_log(LOG_TRACE, log)
    def notice(self, log):
        self.write_log(LOG_NOTICE, log)
    def warning(self, log):
        self.write_log(LOG_WARNING, log)
    def fatal(self, log):
        self.write_log(LOG_FATAL, log)

    def write_log(self, loglevel, log):
        # 判断日志级别
        if loglevel > self.__loglevel:
            return

        # 日志文件
        logname = self.__logname+".log";
        if loglevel==LOG_WARNING or loglevel==LOG_FATAL:
            logname += ".wf";

        # 日志切割方式
        nt = time.time();
        ntlocal = time.localtime(nt)
        tm = ""
        if self.__logslice==SLICE_MONTH:
            tm = time.strftime('%Y%m', ntlocal)
        elif self.__logslice==SLICE_DAY:
            tm = time.strftime('%Y%m%d', ntlocal)
        elif self.__logslice==SLICE_HOUR:
            tm = time.strftime('%Y%m%d-%H', ntlocal)
        if tm != "": logname += "."+tm

        # 时间
        LOG_LEVEL_DICT = {"0":"FATAL", "2":"WARNING", "4":"NOTICE", "8":"TRACE", "16":"DEBUG"}
        loglevelstr = LOG_LEVEL_DICT[str(loglevel)]
        logtime = time.strftime('%m-%d %H:%M:%S', ntlocal)
        logstr = loglevelstr+"\t"+logtime+"\t"+log

        # 输出日志
        #self.write_file(logname, logstr)
        log = self._get_logger(logname)
        log.info(logstr)


    def _get_logger(self, logname):
        key, num = re.subn("[\.|_|-]", "", logname)
        if key in logging.Logger.manager.loggerDict:
            return logging.getLogger(key)
        else:
            logger = logging.getLogger(key)
            logfile = self.__logpath+"/"+logname
            handder  = logging.handlers.RotatingFileHandler(logfile)
            formater = logging.Formatter("%(message)s")
            handder.setFormatter(formater)
            logger.addHandler(handder)
            logger.setLevel(logging.DEBUG)
            return logger


if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)

    print '!!!THE END!!!.'
