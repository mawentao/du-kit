/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: nqc_client.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   nqc_client.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  访问nqcsvr的client
 **/

#ifndef NQC_CLIENT_H
#define NQC_CLIENT_H

#include "ubclient.h"

class NqcClient : public UBClient{
public:
    NqcClient(){}

    ~NqcClient(){}

	/** 处理 */
	int do_process();
};


#endif
