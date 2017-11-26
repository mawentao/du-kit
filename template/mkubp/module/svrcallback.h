/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: svrcallback.h, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   svrcallback.h
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  svrcallback.h
 **/

#ifndef __SVRCALLBACK_H__
#define __SVRCALLBACK_H__

#include "_NAMESPACE__interface.idl.h"
#include "session.h"

namespace _NAMESPACE_{

    /** 服务回调函数类型 */
    typedef int (*ServerCallbackFunc)(
			const nshead_t &head,
			const _NAMESPACE__req_t &req, _NAMESPACE__res_t &res,
			void *userdata, bsl::xcompool &pool);

	/**
	 * @brief  server线程的回调函数
     * @param[in] head : const nshead_t & 请求包nshead
     * @param[in] req  : const _NAMESPACE__req_t & 求包体
     * @param[in/out] res  : _NAMESPACE__res_t & 返回包
     * @param[in/out] usedata : 每个线程独立的辅助数据和内存空间
     * @param[in/out] pool : bsl::xcompool& 每个线程独立的内存池
	 * @return SUCC or FAIL
	**/
	int server_callback(const nshead_t &head,  /* 请求包nshead */
			const _NAMESPACE__req_t &req, /* 请求包体 */
			_NAMESPACE__res_t &res,       /* 返回包 */
			void* usedata,        /* 每个线程独立的辅助数据和内存空间 */
			bsl::xcompool& pool); /* 每个线程独立的内存池 */

	/**
	 * @brief 请求包的合法性检查，若pass将请求包中的数据填入session
	 * @param[in] req_head : const nshead_t & 请求包nshead
	 * @param[in] req_body : const _NAMESPACE__req_t & 请求包体
	 * @param[in/out] session : session_t *
	 * @return SUCC or FAIL
	**/
	int check_req(const nshead_t &req_head, const _NAMESPACE__req_t &req_body, session_t *session);

	/**
	 * @brief 将处理完的session封装成返回包
	 * @param[in] session : session_t *
	 * @param[in/out] res : _NAMESPACE__res_t &
	 * @return SUCC or FAIL
	**/
	int pack_res(session_t *session, _NAMESPACE__res_t &res);

}

#endif
