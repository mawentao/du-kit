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

    /** 使用UBServer框架定义服务类型 */
    typedef Server<_NAMESPACE__req_t, _NAMESPACE__res_t> _KMODULE_NAME_Server;


    /** 主程序类，全局共享数据 */
    class _KMODULE_NAME_Svr{
    public:
        _KMODULE_NAME_Server *m_svr; /**< Common Server */
        ServerCallbackFunc m_svr_callback; /**< 服务请求回调函数 */
        Proc *m_proc;    /**< 请求处理Proc */
        bool m_run_flag; /**< 运行状态标记 */

    public:
        /** 默认构造函数 */
        _KMODULE_NAME_Svr():m_svr(NULL), m_svr_callback(NULL), m_proc(NULL), m_run_flag(false){}

        /** 析构函数关闭日志，释放资源 */
        ~_KMODULE_NAME_Svr(){
            ub_log_close();
            SAFE_DELETE(m_svr);
            SAFE_DELETE(m_proc);
        }

		/**
		 * @brief  进行全局的初始化操作，包括配置、日志、server和各子模块的初始化
		 * @return 成功返回SUCC, 失败返回FAIL
		**/
		int init(comcfg::Configure &cfg, ServerCallbackFunc cb = server_callback);

		/**
		 * @brief  启动icat-keyword服务
		 * @return 成功返回SUCC, 失败返回FAIL
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
		 * @brief  停止icat-keyword服务
		 * @return 成功返回SUCC, 失败返回FAIL
		**/
		int stop(){
			m_svr->stop();
			UB_LOG_TRACE("server is stopped.");
			return SUCC;
		}

	};
};


#endif
