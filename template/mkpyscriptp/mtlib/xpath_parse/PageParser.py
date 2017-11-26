#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   PageParser.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2014-11-27 16:40:45
# @modify: 2014-11-27 16:40:45
# @brief:  PageParser.py
####################################################

import os,sys,re

import urllib
import urllib2 
import lxml.html
import lxml.html.soupparser as soupparser
from XpathLoader import *
from common import *


# 解析网页结构
def extract_page(url, page, domain_attr_dict):
    sres = ""
    domain = getSite(url)
#    print domain
#    print domain_attr_dict
    if domain not in domain_attr_dict:
        #print "can not find the domain template"
        return None

    resultlist = {}
    result = ""
    pagetype_list = domain_attr_dict[domain]
    for pagetype in pagetype_list:
        ptn = re.compile(pagetype["urlpattern"])
        if ptn.search(url):
            all_attr_dict = pagetype["attr"]    
#   html_tree_root = soupparser.fromstring(page)
            html_tree_root = lxml.html.document_fromstring(page.decode("utf-8"))
#            print html_tree_root
            for key in all_attr_dict:
                all_attr_dict[key].dom = html_tree_root
            for key in all_attr_dict:
                all_attr_dict[key].value = all_attr_dict[key].parse(key, domain) 
            result=get_page_result(all_attr_dict)
            break

    return obj2str(result)


# 获取页面解析结果
def get_page_result(all_attr_dict):
    result = {}
    for key in all_attr_dict:
        result[key]=all_attr_dict[key].getvalue();  
    return result




if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)

    print '!!!THE END!!!.'
