/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: _NAMESPACE__func.h, v1.0.0 __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   _NAMESPACE__func.h
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  公共函数定义
 **/

#ifndef ___UPNAMESPACE__FUNC_H__
#define ___UPNAMESPACE__FUNC_H__

#include <bsl/map.h>
#include <cctype>
#include <ub.h>
#include <mc_pack.h>
#include "_NAMESPACE__def.h"
#include "ul_sign.h"

namespace _NAMESPACE_
{
    /** 退出程序 */
	inline void suicide(){
		UB_LOG_FATAL("process suicided.");
		sync();
		kill(getpid(), SIGTERM);
	}

    /** 计算词的签名 */
	uint64_t create_word_sign(const char *word);

	/** 判断是否是文件 */
    inline bool is_file(const char *fname)
    {/*{{{*/
        struct stat file_info;
        stat(fname, &file_info);
        return S_ISREG(file_info.st_mode);
    }/*}}}*/

    /** 获取当前时间 */
    inline void now_time_str(char *tbuf, size_t size, const char *format="%Y-%m-%d %H:%M:%S")
    {/*{{{*/
        struct tm *strt;
        time_t t;
        time(&t);
        strt = localtime(&t);
        strftime(tbuf, size-1, format, strt);
    }/*}}}*/

    /** 打印mcpack成json格式 */
    inline void print_mcpack(mc_pack_t *p, char *buf, uint32_t bufsize)
    {/*{{{*/
        if(p && buf && (0 == mc_pack_pack2json(p, buf, bufsize))){
            printf("mcpack(%u) : %s\n", mc_pack_get_size(p), buf);
        }
    }/*}}}*/

    #if 0
    /** 将字符串转为32位整型 */
	int char_to_uint32(char * str, uint32_t & value);

	/**
	 * @brief 计算时间差
	 * @param[in] s     起始时间
	 * @param[in] e     终止时间
	 * @return          时间差, double 型, 单位为秒
	 */
	inline double timediff(const struct timeval &s, const struct timeval &e){
		return (double)((e.tv_sec - s.tv_sec) * 1E6 + e.tv_usec - s.tv_usec) / 1E6;
	}

	/**
	 * @brief 计算时间差, 返回毫秒值
	 * @param[in] s     起始时间
	 * @param[in] e     终止时间
	 * @return          时间差, int 型, 单位为毫秒
	**/
	inline int timediff_ms(const struct timeval &s, const struct timeval &e){
		return (e.tv_sec - s.tv_sec) * 1000 + (e.tv_usec - s.tv_usec) / 1000;
	}


	/**
	 * @brief 打印版本号与编译时间
	**/
	void print_version();

	/**
	 * @brief 字符串拆分，会修改str的内容，将原串分割为各个字串arr中记录各个字串首地址
	 * @copy from apache src by linxiaobin
	 * @param [in/out] str   : char* 原串
	 * @param [in] dem   : char* 分隔符
	 * @param [out] arr   : char*[] 生成的数组，注意只有指针，指向的是str中的空间
	 * @param [in/out] len   : int* 限制生成的arr的元素上限，函数完成时，保存实际的arr元素个数
	 **/
	void split_string(char *str, char *dem, char *arr[], int *len);

	/**
	 * @brief 字符串拆分，会修改str的内容，将原串分割为各个字串arr中记录各个字串首地址
	 * @copy from apache src by linxiaobin
	 * @param [in/out] str   : char* 原串
	 * @param [in] dem   : char* 分隔符
	 * @param [out] arr   : char*[] 生成的数组，注意只有指针，指向的是str中的空间
	 * @param [in/out] len   : int* 限制生成的arr的元素上限，函数完成时，保存实际的arr元素个数
	 **/
	void split_string_2(char *str, char *dem, char *arr[], int *len);
	/**
	 * @brief 在原来的字符串拆分基础上改进，len指定期望获得的最大段数（原来的函数会将后面的字符串中的dem干掉，而现在的版本保留最后一段中的dem）
	 *
	 * @param str   : char*
	 * @param dem   : char*
	 * @param arr   : char*[]
	 * @param len   : int*
	 * @return  void
	 * @retval
	 * @author wangwf
	 * @date 2012/07/25 17:10:12
	**/
	void split_string_3(char *str, char *dem, char *arr[], int *len);


	/**
	 * @brief 获取文件更新时间
	 * @param[in] fn   文件名
	 * @param[out] mt  时间戳
	 * @return
	 *      - SUCC 成功
	 *      - FAIL 失败
	**/
	int get_mtime(char *fn, time_t &mt);
    #endif
};

#endif


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
