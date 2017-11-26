/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: _MODULE_NAME_.cpp, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   _MODULE_NAME_.cpp
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  _MODULE_NAME_.cpp
 **/

#include "_MODULE_NAME_svr.h"
#include "procs/main_proc.h"


/** ��ʼ���û��ռ䣨ÿ���̶߳�����session�� */
static int userdata_init(uint32_t ithrd, void *shared_buf)
{
    _NAMESPACE_::session_t **session = (_NAMESPACE_::session_t **)shared_buf;
    *session = new(std::nothrow) _NAMESPACE_::session_t();
    if(!*session){
        UB_LOG_FATAL("create _NAMESPACE_::session_t fail. [thread:%u]", ithrd);
        _NAMESPACE_::suicide();
    }
    return _NAMESPACE_::SUCC;
}


/** ��ʼ���������� */
int _NAMESPACE_::_KMODULE_NAME_Svr::init(comcfg::Configure &conf, ServerCallbackFunc cb)
{
	//1. ���ûص�����
	m_svr_callback = cb;

    try{
    //2. ��������ʼ��ubserver
        m_svr = new(std::nothrow) _KMODULE_NAME_Server();
        if(!m_svr){
            UB_LOG_FATAL("_KMODULE_NAME_Svr_::init(): new _KMODULE_NAME_Server() fail!");
            return _NAMESPACE_::FAIL;
        }
        int ret = m_svr->init(conf["_MODULE_NAME_"], sizeof(_NAMESPACE_::session_t**), userdata_init, NULL, NULL);
        if(0 != ret){
            UB_LOG_FATAL("_KMODULE_NAME_Svr_::init(): init server fail. [ret : %d]", ret);
            _NAMESPACE_::suicide();
        }

	//3. ��������ʼ��main_proc
		m_proc = new(std::nothrow) _NAMESPACE_::MainProc();
		if(!m_proc){
			UB_LOG_FATAL("_KMODULE_NAME_Svr_::init(): new MainProc fail.");
			return _NAMESPACE_::FAIL;
		}
		ret = m_proc->init(conf);
		if(_NAMESPACE_::SUCC != ret){
			UB_LOG_FATAL("_KMODULE_NAME_Svr_::init(): init MainProc fail.");
			return _NAMESPACE_::FAIL;
		}
	}catch (bsl::Exception& e){
		UB_LOG_FATAL("Exception when get configure: %s [%s:%d][%s]\n%s",
				e.what(), e.file(), e.line(), e.name(), e.stack());
		return _NAMESPACE_::FAIL;
	}

	return _NAMESPACE_::SUCC;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
