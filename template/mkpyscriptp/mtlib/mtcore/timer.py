#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   timer.py
# @author: mawentao(mawt@youzu.com)
# @create: 2015-05-27 20:08:00
# @modify: 2015-05-27 20:08:00
# @brief:  ���뼶��ʱģ��
####################################################

import os
import sys
sys.path.append(os.path.dirname(__file__)+"/../")
import time
from env import *

class Timer:

    timemap = {}

    # ��ȡ΢��ʱ���
    @staticmethod
    def get_micro_time():
        return time.time()

    # ��¼��ʼʱ��
    @staticmethod
    def begin(key):
        t = Timer.get_micro_time()
        Timer.timemap[key] = {"s":t,"e":t}
    
    # ��¼����ʱ��
    @staticmethod
    def end(key):
        t = Timer.get_micro_time()
        if not Timer.timemap.has_key(key):
            Timer.timemap[key] = {"s":t,"e":t}
        else:
            Timer.timemap[key]["e"] = t

    # ��ȡʱ���
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
