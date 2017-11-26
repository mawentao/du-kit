#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   BaseSpider.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2015-06-15 13:54:29
# @modify: 2015-06-15 13:54:29
# @brief:  BaseSpider.py
####################################################

import os
import sys

from BaseScript import *


class BaseSpider(BaseScript):
    def __init__(self):
        BaseScript.__init__(self)
        # 时间戳
        self.timestamp = int(time.time())
        # 抓取队列
        self.crawlQueue = []
        self.maxCrawlQueueSize = 100
        # 存储队列
        self.storeQueue = []
        self.maxStoreQueueSize = 100

    # 添加到抓取队列，队列满后抓取
    def append_crawl_queue(self, url, params):
        item = (url, params)
        self.crawlQueue.append(item)
        if len(self.crawlQueue) >= self.maxCrawlQueueSize:
            self.crawl_and_parse()
            self.crawlQueue = []
            
    # 抓取并解析
    def crawl_and_parse(self):
        if len(self.crawlQueue)==0:
            return
        for itme in self.crawlQueue:
            url,params = itme
            # 抓取
            downfile = self.crawl(url, params)
            if (not os.path.exists(downfile)) or os.path.getsize(downfile)<10:
                log = "crawl fail! [url:%s] [ret:文件抓取失败]" % (url.encode("GBK"))
                if os.path.exists(downfile):
                    os.remove(downfile)
                self.LOG_TRACE(log)
                continue
            # 解析
            obj = self.parse(downfile, params)
            if not obj:
                log = "crawl fail! [url:%s] [ret:文件解析失败]" % (url.encode("GBK"))
                self.LOG_TRACE(log)
                continue
            else:
                self.storeQueue.append(obj)
                if len(self.storeQueue) > self.maxStoreQueueSize:
                    self.batsave()
                    self.storeQueue = []
            log = "crawl success! [url:%s] [ret:抓取&解析成功]" % (url.encode("GBK"))
            self.LOG_TRACE(log)

    # 执行脚本
    def execute(self, args):
        if len(args)>0:
            nd = args[0]
            ta = time.strptime(nd, "%Y%m%d")
            self.timestamp = int(time.mktime(ta))
        #1. 生成&获取待抓取的urllist
        self.generate()
        #2. 抓取&解析
        self.crawl_and_parse()
        #3. 批量保存解析后的数据
        if len(self.storeQueue) > 0:
            self.batsave()

    # 生产&获取待抓取urllist，此过程中，调用append_crawl_queue函数将url添加到抓取队列
    def generate(self): pass

    # 抓取url，返回抓取的文件名（文件要保存下来）
    def crawl(self, url, params): pass

    # 解析抓取的文件，返回解析后的字典对象或false
    def parse(self, downfile, params): pass

    # 批量保存解析后的数据（从self.storeQueue中取数据）
    def batsave(self): pass

