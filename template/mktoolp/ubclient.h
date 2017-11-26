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


class UBClient{
public:
    UBClient(){}

    ~UBClient(){}

	/** 初始化 */
    int init(comcfg::ConfigUnit &conf);

	/** 处理 */
	int do_process();

private:
    UBClient(const UBClient&);
    UBClient& operator=(const UBClient&);

	ConnectPool m_connect_pool;    //!< 连接池
	uint32_t read_write_time;	   //!< 读写超时时间

    char *m_pack_buf;    //!< 辅助内存
    char *m_tmp_buf;     //!< 辅助内存
    char *m_json_buf;    //!< 辅助内存
    uint32_t m_bufsize;  //!< 辅助内存大小
};


#endif
