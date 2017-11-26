/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: ubclient.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   ubclient.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  ubclient.cpp
 **/

#include "ubclient.h"


/** 初始化ubclient_conf_t */
int ubclient_conf_t::init(comcfg::ConfigUnit &cfg)
{/*{{{*/
    //1. 读取配置
    read_write_time = cfg["read_write_time"].to_uint32();
    mcpack_buf_size = cfg["mcpack_buf_size"].to_uint32();
    sock_perserver = cfg["sock_perserver"].to_uint32();
    connect_retry = cfg["connect_retry"].to_uint32();
    long_connect = cfg["long_connect"].to_uint32();
    uint32_t port = cfg["port"].to_uint32();
    uint32_t ctimesout = cfg["ctimesout"].to_uint32();
    uint32_t ip_size = cfg["ip"].size();
    server_size = ip_size;

    bsl::string ip[32];
    for(uint32_t i = 0; i < ip_size; i++){
        ip[i] = cfg["ip"][i].to_cstr();
    }

    //2. 初始化连接池配置
    server_conf = (ConnectPool::server_conf_t*)calloc(sizeof(ConnectPool::server_conf_t), ip_size);
    if(NULL == server_conf){
        UB_LOG_WARNING("can not allocate server_conf memory");
        return -1;
    }

    for (uint32_t iip = 0; iip < ip_size; ++iip){
        snprintf(server_conf[iip].addr, sizeof(server_conf[iip].addr),"%s", ip[iip].c_str());
        server_conf[iip].port = port;
        server_conf[iip].ctimeout_ms = ctimesout;
    }

    return 0;
}/*}}}*/


/** UBClient初始化函数 */
int UBClient::init(ubclient_conf_t &ubclient_conf)
{/*{{{*/
    //1. 读取配置
    this->read_write_time = ubclient_conf.read_write_time;
    this->m_bufsize = ubclient_conf.mcpack_buf_size;

    //2. 初始化连接池
    try{
        if(NULL == ubclient_conf.server_conf || ubclient_conf.sock_perserver == 0 || ubclient_conf.connect_retry == 0){
            UB_LOG_WARNING("input data error! [sock_perserver:%u, connect_retry:%u]", \
                            ubclient_conf.sock_perserver, ubclient_conf.connect_retry );
            return -1;
        }

        //!< 初始化connect pool
        int ret = m_connect_pool.Init(ubclient_conf.server_conf,
                      ubclient_conf.server_size, ubclient_conf.sock_perserver,
                      ubclient_conf.connect_retry, ubclient_conf.long_connect == 1);
        if(ret < 0){
            UB_LOG_WARNING("create connect pool failed. [ret=%d]", ret);
            return -1;
        }

        m_connect_pool.SetLogInfo("common connectpool", UL_LOG_WARNING, UL_LOG_NOTICE);

    }catch(...){
        UB_LOG_WARNING("exception in connect_pool initialization! [ip_size:%u, sock_perserver:%u, connect_retry:%u]", \
                ubclient_conf.server_size, ubclient_conf.sock_perserver, ubclient_conf.connect_retry );
        return -1;
    }

    //3. 初始化辅助内存
    m_pack_buf = new char[m_bufsize];
    if(!m_pack_buf){
        UB_LOG_FATAL("create m_pack_buf fail");
        return -1;
    }
    m_tmp_buf = new char[m_bufsize];
    if(!m_tmp_buf){
        UB_LOG_FATAL("create m_tmp_buf fail");
        return -1;
    }
    m_json_buf = new char[m_bufsize];
    if(!m_json_buf){
        UB_LOG_FATAL("create m_json_buf fail");
        return -1;
    }

    return 0;
}/*}}}*/


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
