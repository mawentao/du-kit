/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: svrcallback.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   svrcallback.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  请求回调函数
 **/

#ifndef __SVRCALLBACK_H__
#define __SVRCALLBACK_H__

#include "g_data.h"


/**
 * @brief server_t 回调函数
 * @param &req_nshead   请求包头
 * @param req-pack      请求包体
 * @param tmpbuf        缓冲区
 * @param tmpsize       缓冲区大小
 * @return FUN_SUCC or FUN_FAIL
 */
int svr_callback(int sock, nshead_t *head, mc_pack_t *body, char *tmpbuf, size_t tmpsize);


#endif
