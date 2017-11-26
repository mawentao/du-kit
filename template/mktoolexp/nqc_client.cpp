/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: nqc_client.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   nqc_client.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  nqc_client.cpp
 **/

#include "nqc_client.h"

/** 处理 */
int NqcClient::do_process()
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
        printf("nqc-req-pack : %s\n", m_json_buf);
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
        printf("nqc-res-pack : %s\n", m_json_buf);
    }
    // */

    res = 0;
OUT:
    //释放连接
    m_connect_pool.FreeSocket(sock, true);
    return res;

}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
