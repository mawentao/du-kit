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
 * @brief  ����**ubsvr�Ŀͻ���
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

	/** ��ʼ�� */
    int init(comcfg::ConfigUnit &conf);

	/** ���� */
	int do_process();

private:
    UBClient(const UBClient&);
    UBClient& operator=(const UBClient&);

	ConnectPool m_connect_pool;    //!< ���ӳ�
	uint32_t read_write_time;	   //!< ��д��ʱʱ��

    char *m_pack_buf;    //!< �����ڴ�
    char *m_tmp_buf;     //!< �����ڴ�
    char *m_json_buf;    //!< �����ڴ�
    uint32_t m_bufsize;  //!< �����ڴ��С
};


#endif
