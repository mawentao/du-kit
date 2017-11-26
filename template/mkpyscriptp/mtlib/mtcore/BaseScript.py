#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   BaseScript.py
# @author: mawentao(mawt@youzu.com)
# @create: 2015-05-26 20:36:05
# @modify: 2015-05-26 20:36:05
# @brief:  BaseScript.py
####################################################

import os
import sys

from timer import *

class BaseScript:
    def __init__(self):
        self.scriptName = self.__class__.__name__

    def run(self, args):
        self.__beforeRun(args)
        self.execute(args)
        self.__afterRun()

    def __beforeRun(self, args):
        Timer.begin(self.scriptName)
        a = ",".join(args)
        log = "start run [args:%s]" % (a)
        self.LOG_TRACE(log)

    def __afterRun(self):
        Timer.end(self.scriptName)
        diff = Timer.get(self.scriptName)
        log = "finish run [time:%s]" % (diff)
        self.LOG_TRACE(log)

    def execute(self, args):
        print "execute"

    def LOG_NOTICE(self, str):
        self.__write_log("notice", str)
    def LOG_TRACE(self, str):
        self.__write_log("trace", str)
    def LOG_WARNING(self, str):
        self.__write_log("warning", str)
    def LOG_FATAL(self, str):
        self.__write_log("fatal", str)
    def LOG_DEBUG(self, str):
        self.__write_log("debug", str)
    def __write_log(self, level, str):
        log = Env.getlog(self.scriptName)
        if (level=='trace'):
            log.trace(str)
        elif level=='notice':
            log.notice(str)
        elif level=='warning':
            log.warning(str)
        elif level=='fatal':
            log.fatal(str)
        else:
            log.debug(str)
