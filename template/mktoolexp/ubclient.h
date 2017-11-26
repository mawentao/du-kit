/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: ubclient.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   ubclient.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  访问**ubsvr的客户端
 **/

#ifndef UBCLIENT_H
#define UBCLIENT_H

#include <bsl/string.h>
#include <Configure.h>
#include <connectpool.h>
#include <mc_pack.h>
#include <ub.h>

/** ubclient配置数据结构 */
struct ubclient_conf_t{
	ConnectPool::server_conf_t *server_conf;  //!< connect pool server conf
	uint32_t server_size;		//!< server size
	uint32_t sock_perserver;	//!< sock_perserver
	uint32_t connect_retry;		//!< connect_retry
	uint32_t long_connect;		//!< long_connect
	uint32_t read_write_time;	//!< read_write_time
	uint32_t mcpack_buf_size;	//!< mcpack_buf_size

	/** 初始化 */
    int init(comcfg::ConfigUnit &conf);
};


class UBClient{
public:
    UBClient(){}

    virtual ~UBClient(){}

	/** 初始化 */
    int init(ubclient_conf_t &ubclient_conf);

protected:
	ConnectPool m_connect_pool;    //!< 连接池
	uint32_t read_write_time;	   //!< 读写超时时间

    char *m_pack_buf;    //!< 辅助内存
    char *m_tmp_buf;     //!< 辅助内存
    char *m_json_buf;    //!< 辅助内存
    uint32_t m_bufsize;  //!< 辅助内存大小
};


#endif
