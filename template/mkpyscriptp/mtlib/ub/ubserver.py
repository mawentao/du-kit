#!/usr/bin/env python
#coding=gbk

####################################################
# @file:   UBServer.py
# @author: mawentao(mawentao@baidu.com)
# @create: 2014-12-01 14:38:58
# @modify: 2014-12-01 14:38:58
# @brief:  Python版本的UBServer封装
####################################################

import os
import sys
sys.path.append(os.path.dirname(__file__)+"/../")
from env import *
#from PyConnectPool import *
from pynshead import *
import SocketServer
import socket
import mcpack
import threading


class ThreadedTCPServer(SocketServer.ForkingMixIn, SocketServer.TCPServer):
    pass


class UBServer:

    SERVER_NAME   = ""
    READ_TIMEOUT  = 1
    WRITE_TIMEOUT = 1

    # 配置服务
    def __init__(self, handler):
        conf = Env.getconf()

        self.ip       = socket.gethostbyname(socket.gethostname())   
        self.port     = conf.getint("ubserver", "port")
        self.address  = (self.ip, self.port)
        self.thread_num = conf.getint("ubserver", "thread_num")
        self.isruning = True
        self.handler  = handler

        # 配置全局参数
        UBServer.SERVER_NAME   = conf.get("ubserver", "server_name")
        UBServer.READ_TIMEOUT  = conf.getfloat("ubserver", "read_timeout");
        UBServer.WRITE_TIMEOUT = conf.getfloat("ubserver", "write_timeout");

        # 配置mcpack
        mcpack.set_default_version(mcpack.mcpackv2)
        mcpack_buffer_size = conf.getint("ubserver", "mcpack_buffer_size")
        mcpack.set_default_buffer_size(mcpack_buffer_size)

        # log
        log = "init UBServer [port:"+str(self.port)+"] [mcpack_size:"+str(mcpack_buffer_size)+"] "+\
              "[thread_num:"+str(self.thread_num)+"] "+\
              "[read_timeout:"+str(UBServer.READ_TIMEOUT)+"s] "+\
              "[write_timeout:"+str(UBServer.WRITE_TIMEOUT)+"s] "
        Env.getlog().notice(log);

    # 启动服务
    def start(self):
        server = ThreadedTCPServer(self.address, self.handler)
        server.allow_reuse_address = True
        self.threads = []
        for thi in range(self.thread_num):
            thread_num = thi+1
            server_thread = threading.Thread(target=server.serve_forever)
            self.threads.append(server_thread)
            server_thread.start()
            print "server start [thread:"+str(thread_num)+"]"
        print "server is running [port:"+str(self.port)+"]"

    # 终止服务
    def stop(self):
        print "Ctrl + C"
        for thi in range(self.thread_num):
            self.threads[thi].kill()
        #ip, port = server.server_address                     
        #print "server is started [port:"+str(port)+"].\n"
        #server.serve_forever()
        #logging.info("start con reciver "+str(ip)+":"+str(port))          




# 请求处理类父类
class UBServerHandler(SocketServer.BaseRequestHandler):

    # 读取请求包
    def recv_pack(self, sock, recv_len):
        timeout = UBServer.READ_TIMEOUT
        start   = time.time()
        data    = sock.recv(recv_len)
        current = time.time()
        
        while len(data)<recv_len and (current-start)<timeout:
            res = sock.recv(recv_len-len(data))
            data+=res
            current=time.time()
            
        if len(data)==recv_len:
            return data
        else:
            return None

    # 发送返回包
    def send_pack(self, sock):
        pack = mcpack.dumps_version(mcpack.mcpackv2, self.resBody)
        self.resHead._body_len=len(pack)
        header = self.resHead.pack()
        sock.send(header)
        sock.send(pack)


    # 处理请求
    def handle(self):
        #1. 获取请求信息
        self.clientIp, self.clientPort = self.client_address
        thd = threading.currentThread()
        self.thread = thd.getName()
        self.reqHead = {}
        self.reqBody = {}
        self.resHead = PyNshead(id=0, version=0, log_id=0, provider=UBServer.SERVER_NAME, reserved=0, body_len=0)
        self.resBody = {}

        log = "REQUEST [IP:"+self.clientIp+"] "
        log += "[thread:"+self.thread+"] "
        
        timeout = UBServer.READ_TIMEOUT + UBServer.WRITE_TIMEOUT
        self.request.settimeout(timeout)

        try:
            #2. 读取请求包head
            header = self.recv_pack(self.request, 36)
            if not header:
                raise Exception("read nshead fail")
            nshead = PyNshead.unpack(header)
            self.reqHead = nshead.todict()
            self.reqHead["provider"] = self.reqHead["provider"].replace('\x00', '')
            log += "[req_head:"+str(nshead)+"] "

            #3. 读取请求包body
            body_len = self.reqHead["body_len"]
            if body_len <=0:
                raise Exception("read body fail")
            dat = self.recv_pack(self.request, body_len)
            if not dat:
                raise Exception("read body fail")
            self.reqBody = mcpack.loads(dat)
            log += "[req_body:"+str(self.reqBody)+"] "

            #4. 回调函数处理请求
            self.callback()

            #5. 发送返回包
            self.send_pack(self.request)

            log += "[res_head:"+str(self.resHead)+"] "
            log += "[res_body:"+str(self.resBody)+"] "

            #9. 关闭连接
            self.request.close()
            Env.getlog().notice(log)
        except Exception as e:
            self.request.close()
            log += "[error:"+str(e)+"]"
            Env.getlog().warning(log)


    # 回调函数（子类覆盖此方法）
    def callback(self):
        pass

