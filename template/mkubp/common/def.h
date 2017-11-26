/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: _NAMESPACE__def.h, v1.0.0 __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   _NAMESPACE__def.h
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  公共定义
 **/

#ifndef ___UPNAMESPACE__DEF_H__
#define ___UPNAMESPACE__DEF_H__

#include <stdint.h>
#include <ub_log.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

namespace _NAMESPACE_
{
	/**
	 * @brief 定义函数的通用返回码
	**/
	const int32_t SUCC = 0;
	const int32_t FAIL = -1;

    /**
     * @brief 定义请求返回码
    **/
    enum {
        RES_SUCC = 0,     //!< 请求正常返回
        RES_INTERNAL_ERR, //!< 服务内部错误
        RES_PARAM_ERR     //!< 请求字段非法
    };

    /** 其他一些常量定义 */
	const uint32_t FILE_NAME_LEN = 1024;
	const uint32_t MC_PACK_VERSION = 2;

	/**
     * @brief: Check Pointer NULL
     *
     * @return
     *  ret: Pointer is NULL
     */
    #ifndef check_null
    #define check_null(var, ret) \
        do \
        { \
            if (NULL == (var)) \
            { \
                UB_LOG_WARNING("check_null failed. [var='%s']", #var); \
                return ret; \
            } \
        } while (0)
    #endif


	#ifndef check_succ
	#define check_succ(ret) \
		do\
		{ \
			if (icat::SUCC != (ret)) \
			{\
				UB_LOG_WARNING("check_succ failed[%s]", #ret);return ret;\
			}\
		} while (0)
	#endif

    /**
     * @brief: Array Item Count
     */
    #ifndef _countof
    #define _countof(_x) (sizeof(_x) / sizeof(_x[0]))
    #endif

    /**
     * @brief: safe close file descriptor
     */
    #ifndef SAFE_CLOSE
    #define SAFE_CLOSE(fd) do{ ::close(fd); fd = -1; } while(0);
    #endif

    /**
     * @brief: safe delete array allocated by new
     */
    #ifndef SAFE_DELETE_ARRAY
    #define SAFE_DELETE_ARRAY(arry_ptr) do{ if(arry_ptr != NULL){delete [] arry_ptr; arry_ptr = NULL;}}while(0);
    #endif

    /**
     * @brief: safe delete pointer allocated by new
     */
    #ifndef SAFE_DELETE
    #define SAFE_DELETE(arry_ptr) do{ if(arry_ptr != NULL){delete  arry_ptr; arry_ptr = NULL;}}while(0);
    #endif

    /**
     * @brief: safe free pointer allocated by malloc/calloc
     */
    #ifndef SAFE_FREE
    #define SAFE_FREE(arry_ptr) do{ if(arry_ptr != NULL){::free(arry_ptr); arry_ptr = NULL;}}while(0);
    #endif

    /**
     * @brief: IN
     */
    #ifndef IN
    #define IN
    #endif

    /**
     * @brief: OUT
     */
    #ifndef OUT
    #define OUT
    #endif
}


#endif
