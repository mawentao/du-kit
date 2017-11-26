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
 * @brief  ÿ���������߳�ʹ�õ�session���ݽṹ����
 **/

#ifndef __SESSION_H__
#define __SESSION_H__

#include "_NAMESPACE__def.h"
#include "_NAMESPACE__func.h"
#include "string.h"
#include <bsl/pool.h>

namespace _NAMESPACE_{

	/** һ�����������ʹ�õ��û��ռ��ڴ� */
	struct session_t{
		//1. ������Ϣ
        nshead_t head;        //!< �������head

        //2. ��������̵ĸ����ڴ�ռ�
		bsl::xcompool *pool;  //!< �����ڴ��

        //3. ������Ϣ
		int32_t ret;          //!< ������ķ�����
	};

    /**
      * @brief ÿ�δ�������ʱ������һ��session�е�����
      * @param session_t *session
      * @return void
      */
    void reset_session(session_t *session);

    /** [For Debug]: print session */
    void print_session(const session_t *session);

}

#endif
