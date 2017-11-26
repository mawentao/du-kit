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
 * @brief  ��������������࣬������Proc��Ļ���
 **/

#ifndef __PROC_H__
#define __PROC_H__

#include <Configure.h>
#include "session.h"

namespace _NAMESPACE_{

    /** ����������࣬������Proc��Ļ��� */
	class Proc{
	public:
		explicit Proc(bool enable=true):m_enable(enable){}

		virtual ~Proc(){}

		/**
		 * @brief Proc��ʼ���ӿ�
		 * @param conf : comcfg::Configure
		 * @return SUCC or FAIL
		**/
		virtual int init(comcfg::Configure &conf) = 0;

		/**
		 * @brief Proc����ӿڣ�ģ�巽��ģʽ��
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
         * @brief Proc����ӿڣ�������븲�Ǵ˷���
         * @param[in/out] session : session_t &
         * @return SUCC or FAIL
        **/
        virtual int do_process(session_t &session) = 0;

        /**
         * @brief Proc�Ƿ��
        **/
        virtual bool is_enable(){
            return m_enable;
        }

        bool m_enable;    //!< Proc�Ĵ򿪱�־

	};

}
#endif
