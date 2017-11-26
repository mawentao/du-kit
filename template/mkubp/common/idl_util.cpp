/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: idl_util.cpp, v1.0.0 __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   idl_util.cpp
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  idl �е����ƺ���ʵ��
 **/

#include "mcpack_idl.h"

namespace _NAMESPACE_
{
    /**
     * @brief
     *      idl external �������ж��ַ����Ƿ񳬹�len�������ƣ�������
     *      �׳� idl::BreakConstrainException �쳣
     * @param [in/out] str   : char const* ������ַ���
     * @param [in/out] len   : long ��������
     * @return  void
    **/
    void length(char const* str, long len)
    {
        if (!str){
            throw idl::BreakConstrainException() << BSL_EARG << "null str";
        }
        if (len < 0){
            throw idl::BreakConstrainException() << BSL_EARG << "negative len";
        }
        if (strlen(str) > (uint64_t)len){
            throw idl::BreakConstrainException() << BSL_EARG << "str too long";
        }
        return;
    }
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
