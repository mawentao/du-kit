#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   mtutil.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2015-06-02 17:23:13
# @modify: 2015-06-02 17:23:13
# @brief:  mtutil.py
####################################################

import os
import sys


# 准备好文件目录
def prepare_dir(dir):
    if not os.path.isdir(dir):
        os.makedirs(dir)

# 分割路径和文件名
def file_name_split(fullname):
    path  = os.path.dirname(fullname)
    fname = fullname[len(path)+1:]
    return (path, fname)



if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)

    path,fname = file_name_split("./data/appannie_top/20150602/urls.gen")

    print path, fname

    print '!!!THE END!!!.'
