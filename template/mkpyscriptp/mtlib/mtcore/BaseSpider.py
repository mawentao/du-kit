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
        # ʱ���
        self.timestamp = int(time.time())
        # ץȡ����
        self.crawlQueue = []
        self.maxCrawlQueueSize = 100
        # �洢����
        self.storeQueue = []
        self.maxStoreQueueSize = 100

    # ��ӵ�ץȡ���У���������ץȡ
    def append_crawl_queue(self, url, params):
        item = (url, params)
        self.crawlQueue.append(item)
        if len(self.crawlQueue) >= self.maxCrawlQueueSize:
            self.crawl_and_parse()
            self.crawlQueue = []
            
    # ץȡ������
    def crawl_and_parse(self):
        if len(self.crawlQueue)==0:
            return
        for itme in self.crawlQueue:
            url,params = itme
            # ץȡ
            downfile = self.crawl(url, params)
            if (not os.path.exists(downfile)) or os.path.getsize(downfile)<10:
                log = "crawl fail! [url:%s] [ret:�ļ�ץȡʧ��]" % (url.encode("GBK"))
                if os.path.exists(downfile):
                    os.remove(downfile)
                self.LOG_TRACE(log)
                continue
            # ����
            obj = self.parse(downfile, params)
            if not obj:
                log = "crawl fail! [url:%s] [ret:�ļ�����ʧ��]" % (url.encode("GBK"))
                self.LOG_TRACE(log)
                continue
            else:
                self.storeQueue.append(obj)
                if len(self.storeQueue) > self.maxStoreQueueSize:
                    self.batsave()
                    self.storeQueue = []
            log = "crawl success! [url:%s] [ret:ץȡ&�����ɹ�]" % (url.encode("GBK"))
            self.LOG_TRACE(log)

    # ִ�нű�
    def execute(self, args):
        if len(args)>0:
            nd = args[0]
            ta = time.strptime(nd, "%Y%m%d")
            self.timestamp = int(time.mktime(ta))
        #1. ����&��ȡ��ץȡ��urllist
        self.generate()
        #2. ץȡ&����
        self.crawl_and_parse()
        #3. ������������������
        if len(self.storeQueue) > 0:
            self.batsave()

    # ����&��ȡ��ץȡurllist���˹����У�����append_crawl_queue������url��ӵ�ץȡ����
    def generate(self): pass

    # ץȡurl������ץȡ���ļ������ļ�Ҫ����������
    def crawl(self, url, params): pass

    # ����ץȡ���ļ������ؽ�������ֵ�����false
    def parse(self, downfile, params): pass

    # �����������������ݣ���self.storeQueue��ȡ���ݣ�
    def batsave(self): pass

