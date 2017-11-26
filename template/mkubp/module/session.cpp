/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: session.cpp, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

#include "session.h"


/** 重置session数据 */
void _NAMESPACE_::reset_session(session_t *session)
{
	session->ret = _NAMESPACE_::RES_PARAM_ERR;
}


/** 调试用，打印session */
void _NAMESPACE_::print_session(const session_t *session)
{
    //1. print nshead
    printf("request nshead info: {\n");
    printf("\tid : %d\n", session->head.id);
    printf("\tversion : %d\n", session->head.version);
    printf("\tlog_id : %d\n", session->head.log_id);
    printf("\tprovider : %s\n", session->head.provider);
    printf("\tmagic_num : %d\n", session->head.magic_num);
    printf("\treserved : %d\n", session->head.reserved);
    printf("\tbody_len : %d\n", session->head.body_len);
    printf("}\n");

}

