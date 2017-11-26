#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   PyConnectPool.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2014-12-01 14:20:51
# @modify: 2014-12-01 14:20:51
# @brief:  connect pool封装
####################################################

from Env import *

import sys,os
import struct
import socket
import random
import traceback
import time
import mcpack
import threading


'''
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

    def __str__(self):
        return self.__dict__.__repr__()
    def __repr__(self):
        return self.__dict__.__repr__()
'''
    
    
class PyConnectPool(object):

    #读超时时间
    READ_TIMEOUT  = 1  
    #写超时时间
    WRITE_TIMEOUT = 1

    '''
    use polling keep same load balancing strategy
    '''
    def __init__(self, addlist, retry=2, timeout=60):
        self.address=addlist
        self.retry=retry
        self.timeout=timeout
        self.serlist=[adder for i in range(self.retry) for adder in self.address]        
    
    # 读取请求数据
    @classmethod
    def recv(cls,clisock, recv_len):
        timeout = PyConnectPool.READ_TIMEOUT
        start=time.time()
        data=clisock.recv(recv_len)
        current=time.time()
        
        while len(data)<recv_len and (current-start)<timeout:
            res=clisock.recv(recv_len-len(data))
            data+=res
            current=time.time()
            
        if len(data)==recv_len:
            return data
        else:
            return None
        
    @classmethod
    def recv_all(cls,clisock,reply=None):
        #header
        header=cls.recv(clisock, 36)
        if not header:
            #logging.info("recv header timeout")
            clisock.close()
            return None
        
        nshead=PyNshead.unpack(header)
        body_len=nshead._body_len
        
        #body
        if body_len <=0:
            #logging.info("recv empty body")
            clisock.close()
            return {}
        
        dat=cls.recv(clisock,body_len)
        
        if not dat:
            #logging.info("recv data timeout,body_len %d" % body_len )
            clisock.close()
            return None
        #send reply
        if reply==None:
            clisock.close()
        else:
            pack=mcpack.dumps_version(mcpack.mcpackv2,reply)
            nshead._body_len=len(pack)
            header=nshead.pack()
            clisock.send(header)
            clisock.send(pack)
            clisock.close()
        
        #pack to json
        bodydict=mcpack.loads(dat)
        return bodydict
    
    # 接收不释放socket
    @classmethod
    def recv_client(cls, clisock):
        # read nshead
        header=cls.recv(clisock, 36)
        if not header:
            #logging.info("recv header timeout")
            clisock.close()
            return None
        nshead   = PyNshead.unpack(header)
        body_len = nshead._body_len
        
        # read body
        if body_len <=0:
            #logging.info("recv empty body")
            clisock.close()
            return {}
        
        dat=cls.recv(clisock, body_len)
        
        if not dat:
            #logging.info("recv data timeout,body_len %d" % body_len )
            clisock.close()
            return None
        
        #pack to json
        bodydict=mcpack.loads(dat)
        #logging.info("recv_body:%s", bodydict)

        return bodydict

    @classmethod
    def send_client(cls, clisock, jsonbody):
        head = PyNshead()
        pack = mcpack.dumps_version(mcpack.mcpackv2,jsonbody)
        head._body_len=len(pack)
        header=head.pack()
        clisock.send(header)
        clisock.send(pack)
        clisock.close()
        #logging.info("send_body:%s", jsonbody)
        #print obj2str(jsonbody,"utf-8", "utf-8")
       
    
    def query(self,head,jsonbody):
        
        #fill body len
        pack=mcpack.dumps_version(mcpack.mcpackv2,jsonbody)
        head._body_len=len(pack)
        header=head.pack()
        
        
        #server list random keep balance
        #just a shuffle
        splice=random.randint(0,len(self.serlist))
        for host,port in self.serlist[splice:]+self.serlist[0:splice]:
            try:
                #every sock in a single thread
                clisock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                clisock.settimeout(self.timeout)
                clisock.setsockopt(socket.IPPROTO_TCP,socket.TCP_NODELAY,1)
                #send and recv                
                clisock.connect((host, port))
                clisock.send(header)
                clisock.send(pack)
                return  self.recv_all(clisock)
            except Exception as e:
                #logging.warn("Exception:%s host:%s ip:%d head:%s req:%s" %(e,host,port,head,jsonbody))
                str_e=traceback.format_exc().replace('\n', ' ')
                #logging.warn(str_e)
            # try next time
            time.sleep(1)
        
        # error occur
        return None

'''
if __name__=='__main__':
    header=PyNshead(1,1,1,"test",0)
#header=PyNshead(1,0,1,"test",177)
    header._body_len=100
    print header
   
        
    iplist=[('10.40.31.45',9101)]
    
    cpool=PyConnectPool(iplist)
    req={'target_url':sys.argv[1], "loadflag":"images:0 plugin:1 subframes:1 javascript:1 css:0 javascript_output:1 css_output:0 images_output:0 subframes_output:1"}
    
    print obj2str(cpool.query(header, req)).decode('utf-8').encode("utf-8")
''' 



if __name__=='__main__':
    argv = sys.argv
    argc = len(argv)

    print Env.isDebug()

    print '!!!THE END!!!.'
