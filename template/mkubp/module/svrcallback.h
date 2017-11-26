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

    /** ����ص��������� */
    typedef int (*ServerCallbackFunc)(
			const nshead_t &head,
			const _NAMESPACE__req_t &req, _NAMESPACE__res_t &res,
			void *userdata, bsl::xcompool &pool);

	/**
	 * @brief  server�̵߳Ļص�����
     * @param[in] head : const nshead_t & �����nshead
     * @param[in] req  : const _NAMESPACE__req_t & �����
     * @param[in/out] res  : _NAMESPACE__res_t & ���ذ�
     * @param[in/out] usedata : ÿ���̶߳����ĸ������ݺ��ڴ�ռ�
     * @param[in/out] pool : bsl::xcompool& ÿ���̶߳������ڴ��
	 * @return SUCC or FAIL
	**/
	int server_callback(const nshead_t &head,  /* �����nshead */
			const _NAMESPACE__req_t &req, /* ������� */
			_NAMESPACE__res_t &res,       /* ���ذ� */
			void* usedata,        /* ÿ���̶߳����ĸ������ݺ��ڴ�ռ� */
			bsl::xcompool& pool); /* ÿ���̶߳������ڴ�� */

	/**
	 * @brief ������ĺϷ��Լ�飬��pass��������е���������session
	 * @param[in] req_head : const nshead_t & �����nshead
	 * @param[in] req_body : const _NAMESPACE__req_t & �������
	 * @param[in/out] session : session_t *
	 * @return SUCC or FAIL
	**/
	int check_req(const nshead_t &req_head, const _NAMESPACE__req_t &req_body, session_t *session);

	/**
	 * @brief ���������session��װ�ɷ��ذ�
	 * @param[in] session : session_t *
	 * @param[in/out] res : _NAMESPACE__res_t &
	 * @return SUCC or FAIL
	**/
	int pack_res(session_t *session, _NAMESPACE__res_t &res);

}

#endif
