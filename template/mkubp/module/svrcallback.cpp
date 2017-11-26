/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: svrcallback.cpp, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   svrcallback.cpp
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  svrcallback.cpp
 **/

#include "svrcallback.h"
#include "_MODULE_NAME_svr.h"

using namespace _NAMESPACE_;

/** 全局主类对象(定义在main.cpp中) */
extern _KMODULE_NAME_Svr g__MODULE_NAME__svr;

/** 检查请求包头 */
static int check_head(const nshead_t &req_head, session_t *session)
{
	return _NAMESPACE_::SUCC;
}

/** 检查请求包体 */
static int check_body(const _NAMESPACE__req_t &req_body, session_t *session)
{
    return _NAMESPACE_::SUCC;
}

/** 请求包的合法性检查 */
int _NAMESPACE_::check_req(const nshead_t &req_head, const _NAMESPACE__req_t &req_body, session_t *session)
{
	//1. check request's head
	if (_NAMESPACE_::SUCC != check_head(req_head, session)){
		UB_LOG_WARNING("check request's head error!");
		session->ret = -1;
		return _NAMESPACE_::FAIL;
	}

	//2. check request's body
	if (_NAMESPACE_::SUCC != check_body(req_body, session)){
		UB_LOG_WARNING("check request's body error!");
		session->ret = -1;
		return _NAMESPACE_::FAIL;
	}

	return _NAMESPACE_::SUCC;
}


/** 将处理完的session封装成返回包 */
int _NAMESPACE_::pack_res(session_t *session, _NAMESPACE__res_t &res)
{
	return _NAMESPACE_::SUCC;
}


/** 请求回调函数 */
int _NAMESPACE_::server_callback(const nshead_t &head,
        const _NAMESPACE__req_t &req,
        _NAMESPACE__res_t &res,
        void* userdata, bsl::xcompool& pool)
{
    //1. 访问统计
    UB_LOG_TRACE("[MONITOR]: receive_a_request. [provider:%s]", head.provider);

    //2. 重置session
    session_t *session = *(session_t **)userdata;
    reset_session(session);
    session->pool = &pool;
    session->head = head;
    //for debug
    printf("receive_a_request. [provider:%s]\n", head.provider);
    print_session(session);

    //3. 请求包的合法性检查 & 填充session的请求信息
	if (_NAMESPACE_::SUCC != check_req(head, req, session)){
		UB_LOG_WARNING("server_callback: request is not valid!");
		goto out;
	}

	//4. 处理请求，处理的结果都保存在session中
	if (_NAMESPACE_::SUCC != g__MODULE_NAME__svr.m_proc->process(*session)){
		UB_LOG_WARNING("server_callback: MainProc process error!");
		goto out;
	}

	//5. 将处理完的session打包
	if (_NAMESPACE_::SUCC != pack_res(session, res)){
		UB_LOG_WARNING("server_callback: pack result fail!");
		goto out;
	}

out:
    res.set_status(session->ret);
	return _NAMESPACE_::SUCC;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
