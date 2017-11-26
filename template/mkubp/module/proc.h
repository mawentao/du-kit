/***************************************************************************
 *
 * Copyright (c) 2012 Baidu.com, Inc. All Rights Reserved
 * $Id: proc.h, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   proc.h
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  定义请求处理抽象类，所有子Proc类的基类
 **/

#ifndef __PROC_H__
#define __PROC_H__

#include <Configure.h>
#include "session.h"

namespace _NAMESPACE_{

    /** 请求处理抽象类，所有子Proc类的基类 */
	class Proc{
	public:
		explicit Proc(bool enable=true):m_enable(enable){}

		virtual ~Proc(){}

		/**
		 * @brief Proc初始化接口
		 * @param conf : comcfg::Configure
		 * @return SUCC or FAIL
		**/
		virtual int init(comcfg::Configure &conf) = 0;

		/**
		 * @brief Proc处理接口（模板方法模式）
		 * @param session : session_t &
		 * @return SUCC or FAIL
		 **/
		int process(session_t &session){
            if(!is_enable()){
                return SUCC;
            }
            return do_process(session);
        }

    protected:
        /**
         * @brief Proc处理接口，子类必须覆盖此方法
         * @param[in/out] session : session_t &
         * @return SUCC or FAIL
        **/
        virtual int do_process(session_t &session) = 0;

        /**
         * @brief Proc是否打开
        **/
        virtual bool is_enable(){
            return m_enable;
        }

        bool m_enable;    //!< Proc的打开标志

	};

}
#endif
