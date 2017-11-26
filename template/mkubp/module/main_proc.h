/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: main_proc.h, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   main_proc.h
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  主请求处理 proc
 **/

#ifndef __MAIN_PROC_H__
#define __MAIN_PROC_H__

#include "../proc.h"


namespace _NAMESPACE_{

    /** 主请求处理 proc 类 */
	class MainProc : public Proc{
	public:
		MainProc(){}

		~MainProc(){}

		int init(comcfg::Configure &conf);

    protected:

		int do_process(session_t &session);
	};

};

#endif
