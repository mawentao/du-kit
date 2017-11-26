/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: _NAMESPACE__func.cpp, v1.0.0 __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   _NAMESPACE__func.cpp
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  _NAMESPACE__func.cpp
 **/

#include "auto_res.h"
#include "_NAMESPACE__func.h"

namespace _NAMESPACE_
{
    /** 计算词的签名 */
    uint64_t create_word_sign(const char *word)
    {
		if(NULL == word){
			UB_LOG_WARNING("input word is NULL!");
			return 0;
		}
		char temp_word[256] = "";
		uint32_t sign1,
				 sign2;
		strcpy(temp_word, word);

		creat_sign_f64(temp_word, strlen(temp_word), &sign1, &sign2);
		uint64_t sign64 = sign1;
		sign64 = (sign64<<32) + sign2;  //!< sign2为低32bit, sign1为高32bit
		return sign64;
	}


}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
