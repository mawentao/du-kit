#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   timer.py
# @author: mawentao(mawt@youzu.com)
# @create: 2015-05-27 20:08:00
# @modify: 2015-05-27 20:08:00
# @brief:  毫秒级计时模块
####################################################

import os
import sys
sys.path.append(os.path.dirname(__file__)+"/../")
import time
from env import *

class Timer:

    timemap = {}

    # 获取微妙时间戳
    @staticmethod
    def get_micro_time():
        return time.time()

    # 记录起始时间
    @staticmethod
    def begin(key):
        t = Timer.get_micro_time()
        Timer.timemap[key] = {"s":t,"e":t}
    
    # 记录结束时间
    @staticmethod
    def end(key):
        t = Timer.get_micro_time()
        if not Timer.timemap.has_key(key):
            Timer.timemap[key] = {"s":t,"e":t}
        else:
            Timer.timemap[key]["e"] = t

    # 获取时间差
    @staticmethod
    def get(key):
        if not Timer.timemap.has_key(key):
            return 0
        else:
            dif = Timer.timemap[key]["e"] - Timer.timemap[key]["s"]
            res = "%.2f s" % (dif)
            if (dif < 1):
                d = dif * 1000
                res = "%.2f ms" % (d)
            if (Env.isDebug()):
                log = "%s [%s]" % (key, res)
                Env.getlog("timer").notice(log);
            return res


if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)
    Timer.begin("dem")
    import time
    time.sleep(0.1)
    Timer.end("dem");
    print Timer.get("dem")

    Timer.begin("dem")
    time.sleep(1)
    Timer.end("dem");
    print Timer.get("dem")
