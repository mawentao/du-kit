#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   DBO.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2015-06-16 16:05:40
# @modify: 2015-06-16 16:05:40
# @brief:  DBO.py
####################################################

import os
import sys
sys.path.append(os.path.dirname(__file__)+"/../")
from env import *
from mtcore import *
import MySQLdb

class DBO:
    _conn = None
    _cur  = None

    def __init__(self, host, port, user, passwd, db):
        self._conn = MySQLdb.connect(host=host, user=user, passwd=passwd, db=db, port=port,charset='utf8')

    def __del__(self):
        if self._conn:
            self._conn.close()

    # 执行sql，返回影响的行数
    def execute(self, sql):
        try:
            Timer.begin("DBO.execute")
            self._cur = self._conn.cursor()
            affectRows = self._cur.execute(sql)
            self._cur.close()
            self._conn.commit()
            Timer.end("DBO.execute") 
            log = "%s [affect_rows:%d] [time:%s]" % (sql, affectRows, Timer.get("DBO.execute"))
            Env.getlog("sql").notice(log);
            return affectRows
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])
        except MySQLdb.Warning, w:
            print "Mysql Warning:%s" % str(w)
        return 0

