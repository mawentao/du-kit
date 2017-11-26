/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: session.h, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   session.h
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  每个请求处理线程使用的session数据结构定义
 **/

#ifndef __SESSION_H__
#define __SESSION_H__

#include "_NAMESPACE__def.h"
#include "_NAMESPACE__func.h"
#include "string.h"
#include <bsl/pool.h>

namespace _NAMESPACE_{

	/** 一次请求处理过程使用的用户空间内存 */
	struct session_t{
		//1. 请求信息
        nshead_t head;        //!< 请求包的head

        //2. 请求处理过程的辅助内存空间
		bsl::xcompool *pool;  //!< 辅助内存池

        //3. 返回信息
		int32_t ret;          //!< 请求处理的返回码
	};

    /**
      * @brief 每次处理请求时，重置一下session中的数据
      * @param session_t *session
      * @return void
      */
    void reset_session(session_t *session);

    /** [For Debug]: print session */
    void print_session(const session_t *session);

}

#endif
