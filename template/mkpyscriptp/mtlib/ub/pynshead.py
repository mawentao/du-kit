#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   pynshead.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2014-12-02 13:34:18
# @modify: 2014-12-02 13:34:18
# @brief:  pynshead.py
####################################################

import os
import sys
import struct


# nshead结构体（pack & unpack）
class PyNshead(object):

    def __init__(self,id=0,version=0,log_id=0,provider="test",reserved=2,body_len=0):
        self._id=id
        self._version=version
        self._log_id=log_id
        self._provider=provider+''.join(['\x00']*(16-len(provider)))
        self._magic_num=0xfb709394
        self._reserved=reserved
        self._body_len=body_len
    
    def pack(self):
        header=[self._id,
                self._version,
                self._log_id,
                self._provider,
                self._magic_num,
                self._reserved,
                self._body_len]
        return struct.pack("2HI16s3I", *header)

    @classmethod
    def unpack(cls,header):
        head=PyNshead()
        (head._id,head._version,head._log_id,head._provider,\
           head._magic_num,head._reserved,head._body_len) \
           =struct.unpack("2HI16s3I", header)
        return head
        
    # 转成dict
    def todict(self):
        dict = {
            "id" : self._id,
            "version"   : self._version,
            "log_id"    : self._log_id,
            "provider"  : self._provider,
            "magic_num" : self._magic_num,
            "reserved"  : self._reserved,
            "body_len"  : self._body_len
        }
        return dict

    # 转成字符串
    def __str__(self):
        format = "{id:%d, version:%d, log_id:%d, provider:'%s', magic_num:%d, reserved:%d, body_len:%d}"
        values = (self._id, self._version, self._log_id, self._provider, self._magic_num, self._reserved, self._body_len)
        resstr = format % values
        return resstr;
        #return self.__dict__.__repr__()

    def __repr__(self):
        return self.__dict__.__repr__()


if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)

    nshead  = PyNshead()
    print nshead

    print '!!!THE END!!!.'
