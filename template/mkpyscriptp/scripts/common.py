#!/usr/bin/env python
#coding=utf-8

####################################################
# @file:   common.py
# @author: _AUTHOR_
# @create: _DATETIME_
# @modify: _DATETIME_
# @brief:  common.py
####################################################

import sys
sys.path.append("../")
from mtlib import *
import json

# 获取抓取文件存储目录
def get_crawl_path(tm):
    ct = time.localtime(tm)
    conf = Env.getconf()
    root_path = conf.get("crawl","root_path")
    prepare_dir(root_path)
    crawl_circle = conf.getint("crawl", "circle")

    crawl_path = root_path
    if crawl_circle>0:
        crawl_path += "/"+time.strftime("%Y%m%d", ct)
        if crawl_circle==2:
            crawl_path += "/"+time.strftime("%H", ct) 
    prepare_dir(crawl_path)
    return crawl_path

# 获取下载后保存文件的全路径文件名
def get_full_down_file(downf, tm):
    crawl_path = get_crawl_path(tm)
    fullname = "%s/%s" % (crawl_path, downf)
    path,name = file_name_split(fullname)
    prepare_dir(path)
    return fullname

# 下载文件
def download(url, downfile):
    if os.path.exists(downfile):
        os.remove(downfile)
    cmd = "wget --no-check-certificate -t 2 -T 3 -O \"%s\" %s" % (downfile, url)
    Env.getlog("download").trace(cmd)
    os.system(cmd)
    
# 获取xpath
def get_xpath(filename):
    conf = Env.getconf()
    xpath_path = conf.get("crawl","xpath_path")
    file = "%s/%s" % (xpath_path, filename)
    if not os.path.isfile(file):
        raise Exception("xpath file is not exist [%s]" % (file))
    return file

# 解析页面
def parse_page(file, xpath_dict):
    if not os.path.isfile(file):
        return False
    fi = open(file,"r")
    content = fi.read()
    fi.close()
    try:
        domain = xpath_dict.keys()[0]
        res = PageParser.extract_page(domain, content, xpath_dict)
        obj = json.loads(res)
        return obj
    except Exception, ex:
        print Exception,":",ex
        return False

# 计算签名
def sign64(bundleId):
    words = bundleId.lower()
    conf = Env.getconf()
    host = conf.get("signsvr", "host") #"127.0.0.1"
    port = conf.getint("signsvr", "port") #9991
    nshead = {
        "id": 1,
        "version": 1,
        "log_id": 1,
        "provider": 0x6b636170646e6573,
        "magic_num": 0xfb709394,
        "reserved": 0,
        "body_len": 0
    }
    try:
        ubclient = UBClient(host, port)
        reqpack = {"words":[words]}
        respack = ubclient.send_and_recv(nshead, reqpack)
        if respack["status"]==0 and len(respack["wordres"])>0:
            sign = respack["wordres"][0]["ws"]
            return sign
    except Exception as e:
        log = "excption in sign64 [%s]" % (str(e))
        Env.getlog().warning(log)
        return 0


if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)

    print sign64("COM.aa")
    print sign64("nike")

    print '!!!THE END!!!.'
