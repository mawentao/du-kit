#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   ubclient.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2015-01-05 11:44:28
# @modify: 2015-01-05 11:44:28
# @brief:  ubclient.py
####################################################

import sys
import mcpack
import socket
import struct
import time
import getopt


class UBClient(object):
    def __init__(self, h, p):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#       self.sock.settimeout(30)
#       self.sock.setsockopt(socket.IPPROTO_TCP,socket.TCP_NODELAY,1) 
        try:
            self.sock.connect((h, p))
        except Exception, e:
            msg = "%s [host:%s] [port:%s]" % (str(e), h, str(p))
            raise Exception(msg)
    
    def recv_all(self):
        head = self.sock.recv(36,socket.MSG_WAITALL)
        #parse head
        (id, verison, log_id,provider,magic_num,reserved,body_len)=struct.unpack("2HI16s3I", head)
        #recv body
        dat = self.sock.recv(body_len)
        rev_len=len(dat)
        while rev_len < body_len:
            dat += self.sock.recv(body_len-rev_len)
            rev_len = len(dat)
        mcpack.set_default_version(mcpack.mcpackv2)
        bodydict = mcpack.loads(dat)
        return bodydict

    def send_and_recv(self, HEAD, jsonbody):
        #封装pack
        pack = mcpack.dumps_version(mcpack.mcpackv2,jsonbody)
        #封装head
        plen=len(pack)
        provider1 = 0x6363 #0x6b636170646e6573
        provider2 = 0x0031
#        head=struct.pack('HHHQQIII',hid,version,log_id,provider1,provider2,magic_num,0,plen)
        head=struct.pack('HHHQQIII', HEAD["id"],HEAD['version'],HEAD['log_id'],HEAD['provider'],
                          provider2,HEAD['magic_num'],HEAD['reserved'],plen)
        try:
            self.sock.send(head)
            self.sock.send(pack)
            time.sleep(0.1)
            return  self.recv_all()
        except Exception, e:
            print e
            raise e



if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)

    print '!!!THE END!!!.'
