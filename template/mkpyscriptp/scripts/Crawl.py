#!/usr/bin/env python
#coding=utf-8

####################################################
# @file:   Crawl.py
# @author: mawentao(mawt@youzu.com)
# @create: 2015-12-17 11:08:37
# @modify: 2015-12-17 11:08:37
# @brief:  Crawl.py
####################################################

from common import *

class Crawl(BaseScript):
    def __init__(self):
        BaseScript.__init__(self)
        self.scriptName = self.__class__.__name__
        # 时间戳
        self.timestamp = int(time.time()) 
        
    def execute(self, args):
        url = "http://www.baidu.com"
        # 抓取网页源码 
        print 'crawl '+url
        downfile=get_full_down_file("baidu.html",self.timestamp)
        if not os.path.isfile(downfile):
            download(url, downfile)
        # 结构化解析
        xpath = get_xpath("baidu.xml")
        xpath_dict = XpathLoader.load_xpath_from_file(xpath)
        print xpath_dict
        res = parse_page(downfile, xpath_dict)
        print res
        print "title: ",res["title"][0]

