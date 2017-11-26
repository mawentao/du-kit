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
 * @brief  idl 中的限制函数实现
 **/

#include "mcpack_idl.h"

namespace _NAMESPACE_
{
    /**
     * @brief
     *      idl external 函数，判断字符串是否超过len长度限制，超过则
     *      抛出 idl::BreakConstrainException 异常
     * @param [in/out] str   : char const* 待检查字符串
     * @param [in/out] len   : long 长度限制
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
