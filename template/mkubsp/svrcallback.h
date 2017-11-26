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
 * @brief  ����ص�����
 **/

#ifndef __SVRCALLBACK_H__
#define __SVRCALLBACK_H__

#include "g_data.h"


/**
 * @brief server_t �ص�����
 * @param &req_nshead   �����ͷ
 * @param req-pack      �������
 * @param tmpbuf        ������
 * @param tmpsize       ��������С
 * @return FUN_SUCC or FUN_FAIL
 */
int svr_callback(int sock, nshead_t *head, mc_pack_t *body, char *tmpbuf, size_t tmpsize);


#endif
