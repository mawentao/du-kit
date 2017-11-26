#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   xpath_loader.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2014-11-27 15:18:49
# @modify: 2014-11-27 15:18:49
# @brief:  xpath_loader.py
####################################################

import os
import urllib
import urllib2 
import lxml.html

from xml.etree import ElementTree
import lxml.etree as etree

from const import *
from common import *


#attribute类，通过递归获取整个子attribute的结0m~D
class attr_object:
    def __init__(self, dom = None, xpath = ""):
        self.dom = dom
        self.xpath = xpath
        self.sub_attr = []
        self.value = ""
        self.name = ""

    def parse(self, name, site):
        self.name = name
        htmlflag = 0
        if self.xpath.find('@basehtml') >= 0:
            self.xpath = self.xpath[0:len(self.xpath)-10]
            htmlflag = 1
        obj_xml = self.dom.xpath(self.xpath)
        #print obj_xml, self.xpath
        if len(self.sub_attr) == 0:
            self.value = []
            i = 0
            for i in range(0,len(obj_xml)):
                if htmlflag:
                    if not (isinstance(obj_xml[i], str)):
                        self.value.append(urllib.quote(etree.tostring(obj_xml[i], encoding='utf-8')))
                    continue
                temp_str = deal_obj(obj_xml[i])
                if self.xpath.find('@href') >= 0 or self.xpath.find('@src') >= 0:
                    domain = getSite(temp_str)
                    temp_domain = ""
                    if domain=="" or ( domain != site and len(domain) < 3):
                        temp_domain = "http://"+site
                        if temp_str[0] != '/':
                            temp_str = '/'+temp_str
                        temp_str = temp_domain+temp_str
                        #logging.warn("%s\t%s\t%s\n", domain, site,temp_str)
                self.value.append(temp_str)
            return self.value
        else:
            self.value = []
            for obj in obj_xml:
                self.value.append({})
                for sub_obj in self.sub_attr:
                    for key in sub_obj:
                        sub_obj[key].dom = obj
                        self.value[-1][key] = sub_obj[key].parse(key,site)
            return self.value
        return None
    def getvalue(self):
        return self.value
    def getvalue_by_name(self, name):
        if self.name == name:
            return self.value
        else:
            if len(self.sub_attr) == 0:
                return ""
            else:
                for sub_obj in self.sub_attr:
                    for key in sub_obj:
                        return sub_obj[key].getvalue_by_name(name)


#递归地解析attributettrib
def parse_attr(attr,attr_dict):
    key = attr.get('name')
    xpath = attr.get('xpath')
    attr_dict[key] = attr_object(None,xpath)
    subattr_list = attr.findall('attr')
    for subattr in subattr_list:
        attr_dict[key].sub_attr.append({})
        parse_attr(subattr,attr_dict[key].sub_attr[-1])
    return SUCC

# 加载xml格式的xpath
def load_xpath(xmlstr):
    domain_attr_dict = {}
    root = ElementTree.fromstring(xmlstr)
    page_list = root.findall('page')
    for page in page_list:
        domain = page.get("domain")
        pattern = page.get("pattern")
        if not domain_attr_dict.has_key(domain):
            domain_attr_dict[domain] = []
        domain_attr_dict[domain].append({})
        domain_attr_dict[domain][-1]["urlpattern"] = pattern 
        domain_attr_dict[domain][-1]["attr"] = {}
        attr_list = page.findall('attr')
        for attr in attr_list:
            parse_attr(attr,domain_attr_dict[domain][-1]["attr"])
    return domain_attr_dict

# 从xml文件加载xpath
def load_xpath_from_file(filename):
    if os.path.isfile(filename) == False:
        return ERR
    xml = open(filename).read()
    try:
        return load_xpath(xml)
    except Exception as e:
        print ("Exception:%s\n", e)
        return ERR





if __name__=='__main__':
    import os
    import sys
    argv = sys.argv
    argc = len(argv)

    print SUCC

    print '!!!THE END!!!.'
