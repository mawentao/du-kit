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


/** 初始化 */
int UBClient::init(comcfg::ConfigUnit &cfg)
{/*{{{*/
    //1. 读取配置
    read_write_time = cfg["read_write_time"].to_uint32();
    uint32_t sock_perserver = cfg["sock_perserver"].to_uint32();
    uint32_t connect_retry = cfg["connect_retry"].to_uint32();
    uint32_t long_connect = cfg["long_connect"].to_uint32();
    uint32_t port = cfg["port"].to_uint32();
    uint32_t ctimesout = cfg["ctimesout"].to_uint32();
    uint32_t ip_size = cfg["ip"].size();

    bsl::string ip[32];
    for(uint32_t i = 0; i < ip_size; i++){
        ip[i] = cfg["ip"][i].to_cstr();
    }

    //2. 初始化连接池配置
    ConnectPool::server_conf_t *server_conf;
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

    //3. 初始化连接池
    try{
        if(NULL == server_conf || 0 == ip_size || sock_perserver == 0 || connect_retry == 0){
            UB_LOG_WARNING("input data error! [size:%u, sock_perserver:%u, connect_retry:%u]", \
                            ip_size, sock_perserver, connect_retry );
            return -1;
        }

        //!< 初始化connect pool
        int ret = m_connect_pool.Init(server_conf, ip_size, sock_perserver, connect_retry, long_connect == 1);
        if(ret < 0){
            UB_LOG_WARNING("create connect pool failed. [ret=%d]", ret);
            return -1;
        }

        m_connect_pool.SetLogInfo("common connectpool", UL_LOG_WARNING, UL_LOG_NOTICE);

    }catch(...){
        UB_LOG_WARNING("exception in connect_pool initialization! [ip_size:%u, sock_perserver:%u, connect_retry:%u]", \
                ip_size, sock_perserver, connect_retry );
        return -1;
    }

    //4. 初始化辅助内存
    m_bufsize = cfg["mcpack_buf_size"].to_uint32();
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


/** 处理 */
int UBClient::do_process()
{
    //1. 封装请求包(head)
    nshead_t* head = (nshead_t*)m_pack_buf;
    head->id = 0x01;
    head->version = 0x100;
    head->reserved = 0;
    head->magic_num = 0xfb709394;
    sprintf(head->provider, "%s", "toolp");

    //2. 封装请求包(body)
    mc_pack_t *p;
    p = mc_pack_open_w(2, m_pack_buf+sizeof(nshead_t), m_bufsize-sizeof(nshead_t), m_tmp_buf, m_bufsize);
    int ret = MC_PACK_PTR_ERR(p);
    if(ret != 0){
        UB_LOG_WARNING("failed to open req mcpack. [ret=%d info='%s']", ret, mc_pack_perror(ret));
        return -1;
    }
    mc_pack_put_uint32(p, "type", 0);
    mc_pack_t *p_parameters = mc_pack_put_array(p, "parameters");
    mc_pack_put_str(p_parameters, NULL, "p1");
    mc_pack_put_str(p_parameters, NULL, "p2");

    head->body_len = mc_pack_get_size(p);

    // /* for debug: 打印请求包
    if(mc_pack_pack2json(p, m_json_buf, m_bufsize) == 0){
        printf("req-pack : %s\n", m_json_buf);
    }
    // */

    //3. 发送请求
    int sock = m_connect_pool.FetchSocket();
    if(sock < 0){
        UB_LOG_WARNING("fetch_socket fail! [socket:%d]", sock);
        return -1;
    }

    int res = -1;    //返回码
    ret = nshead_write(sock, head, m_pack_buf+sizeof(nshead_t), read_write_time);
    if(ret < 0){
        UB_LOG_FATAL("send data to nqc-server fail.");
        goto OUT;
    }

    //4. 接收返回
    nshead_t readhead;
    ret = nshead_read(sock, &readhead, m_pack_buf, m_bufsize, read_write_time);
    if(ret < 0){
        UB_LOG_WARNING("_query_pack():nshead_read error! [ret:%d]", ret);
        goto OUT;
    }

    //5. 处理返回包
    p = mc_pack_open_r(m_pack_buf, m_bufsize, m_tmp_buf, m_bufsize);
    ret = MC_PACK_PTR_ERR(p);
    if(ret){
        UB_LOG_WARNING("failed to read readpack. [ret=%d info='%s']", ret, mc_pack_perror(ret));
        goto OUT;
    }

    // /* for debug: 打印返回包
    if(mc_pack_pack2json(p, m_json_buf, m_bufsize) == 0){
        printf("res-pack : %s\n", m_json_buf);
    }
    // */

    res = 0;
OUT:
    //释放连接
    m_connect_pool.FreeSocket(sock, true);
    return res;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
