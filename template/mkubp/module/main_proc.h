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
 * @brief  �������� proc
 **/

#ifndef __MAIN_PROC_H__
#define __MAIN_PROC_H__

#include "../proc.h"


namespace _NAMESPACE_{

    /** �������� proc �� */
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
