/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: _MODULE_NAME_svr.h, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   _MODULE_NAME_.h
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  _MODULE_NAME_.h
 **/

#ifndef ____UPNAME___SVR_H__
#define ____UPNAME___SVR_H__

#include "bsl/string.h"
#include "bsl/containers/hash/bsl_hashmap.h"
#include "Configure.h"
#include "ub.h"
#include "ubsvr.h"
#include "svrcallback.h"
#include "proc.h"

namespace _NAMESPACE_{

    /** ʹ��UBServer��ܶ���������� */
    typedef Server<_NAMESPACE__req_t, _NAMESPACE__res_t> _KMODULE_NAME_Server;


    /** �������࣬ȫ�ֹ������� */
    class _KMODULE_NAME_Svr{
    public:
        _KMODULE_NAME_Server *m_svr; /**< Common Server */
        ServerCallbackFunc m_svr_callback; /**< ��������ص����� */
        Proc *m_proc;    /**< ������Proc */
        bool m_run_flag; /**< ����״̬��� */

    public:
        /** Ĭ�Ϲ��캯�� */
        _KMODULE_NAME_Svr():m_svr(NULL), m_svr_callback(NULL), m_proc(NULL), m_run_flag(false){}

        /** ���������ر���־���ͷ���Դ */
        ~_KMODULE_NAME_Svr(){
            ub_log_close();
            SAFE_DELETE(m_svr);
            SAFE_DELETE(m_proc);
        }

		/**
		 * @brief  ����ȫ�ֵĳ�ʼ���������������á���־��server�͸���ģ��ĳ�ʼ��
		 * @return �ɹ�����SUCC, ʧ�ܷ���FAIL
		**/
		int init(comcfg::Configure &cfg, ServerCallbackFunc cb = server_callback);

		/**
		 * @brief  ����icat-keyword����
		 * @return �ɹ�����SUCC, ʧ�ܷ���FAIL
		**/
		int start(){
			if (0 == m_svr->run(m_svr_callback)){
				UB_LOG_TRACE("server is running\n");
				m_run_flag = true;
				return SUCC;
			}else{
				UB_LOG_FATAL("server start error!");
				return FAIL;
			}
		}

		/**
		 * @brief  ֹͣicat-keyword����
		 * @return �ɹ�����SUCC, ʧ�ܷ���FAIL
		**/
		int stop(){
			m_svr->stop();
			UB_LOG_TRACE("server is stopped.");
			return SUCC;
		}

	};
};


#endif
