/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: main_proc.cpp, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   main_proc.cpp
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  main_proc.cpp
 **/

#include "main_proc.h"

namespace _NAMESPACE_{

int MainProc::init(comcfg::Configure &conf)
{
	return SUCC;
}

int MainProc::do_process(session_t &session)
{
    session.ret = RES_SUCC;
	return SUCC;
}

}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab

