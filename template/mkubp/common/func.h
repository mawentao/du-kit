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
 * @brief  ������������
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
    /** �˳����� */
	inline void suicide(){
		UB_LOG_FATAL("process suicided.");
		sync();
		kill(getpid(), SIGTERM);
	}

    /** ����ʵ�ǩ�� */
	uint64_t create_word_sign(const char *word);

	/** �ж��Ƿ����ļ� */
    inline bool is_file(const char *fname)
    {/*{{{*/
        struct stat file_info;
        stat(fname, &file_info);
        return S_ISREG(file_info.st_mode);
    }/*}}}*/

    /** ��ȡ��ǰʱ�� */
    inline void now_time_str(char *tbuf, size_t size, const char *format="%Y-%m-%d %H:%M:%S")
    {/*{{{*/
        struct tm *strt;
        time_t t;
        time(&t);
        strt = localtime(&t);
        strftime(tbuf, size-1, format, strt);
    }/*}}}*/

    /** ��ӡmcpack��json��ʽ */
    inline void print_mcpack(mc_pack_t *p, char *buf, uint32_t bufsize)
    {/*{{{*/
        if(p && buf && (0 == mc_pack_pack2json(p, buf, bufsize))){
            printf("mcpack(%u) : %s\n", mc_pack_get_size(p), buf);
        }
    }/*}}}*/

    #if 0
    /** ���ַ���תΪ32λ���� */
	int char_to_uint32(char * str, uint32_t & value);

	/**
	 * @brief ����ʱ���
	 * @param[in] s     ��ʼʱ��
	 * @param[in] e     ��ֹʱ��
	 * @return          ʱ���, double ��, ��λΪ��
	 */
	inline double timediff(const struct timeval &s, const struct timeval &e){
		return (double)((e.tv_sec - s.tv_sec) * 1E6 + e.tv_usec - s.tv_usec) / 1E6;
	}

	/**
	 * @brief ����ʱ���, ���غ���ֵ
	 * @param[in] s     ��ʼʱ��
	 * @param[in] e     ��ֹʱ��
	 * @return          ʱ���, int ��, ��λΪ����
	**/
	inline int timediff_ms(const struct timeval &s, const struct timeval &e){
		return (e.tv_sec - s.tv_sec) * 1000 + (e.tv_usec - s.tv_usec) / 1000;
	}


	/**
	 * @brief ��ӡ�汾�������ʱ��
	**/
	void print_version();

	/**
	 * @brief �ַ�����֣����޸�str�����ݣ���ԭ���ָ�Ϊ�����ִ�arr�м�¼�����ִ��׵�ַ
	 * @copy from apache src by linxiaobin
	 * @param [in/out] str   : char* ԭ��
	 * @param [in] dem   : char* �ָ���
	 * @param [out] arr   : char*[] ���ɵ����飬ע��ֻ��ָ�룬ָ�����str�еĿռ�
	 * @param [in/out] len   : int* �������ɵ�arr��Ԫ�����ޣ��������ʱ������ʵ�ʵ�arrԪ�ظ���
	 **/
	void split_string(char *str, char *dem, char *arr[], int *len);

	/**
	 * @brief �ַ�����֣����޸�str�����ݣ���ԭ���ָ�Ϊ�����ִ�arr�м�¼�����ִ��׵�ַ
	 * @copy from apache src by linxiaobin
	 * @param [in/out] str   : char* ԭ��
	 * @param [in] dem   : char* �ָ���
	 * @param [out] arr   : char*[] ���ɵ����飬ע��ֻ��ָ�룬ָ�����str�еĿռ�
	 * @param [in/out] len   : int* �������ɵ�arr��Ԫ�����ޣ��������ʱ������ʵ�ʵ�arrԪ�ظ���
	 **/
	void split_string_2(char *str, char *dem, char *arr[], int *len);
	/**
	 * @brief ��ԭ�����ַ�����ֻ����ϸĽ���lenָ��������õ���������ԭ���ĺ����Ὣ������ַ����е�dem�ɵ��������ڵİ汾�������һ���е�dem��
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
	 * @brief ��ȡ�ļ�����ʱ��
	 * @param[in] fn   �ļ���
	 * @param[out] mt  ʱ���
	 * @return
	 *      - SUCC �ɹ�
	 *      - FAIL ʧ��
	**/
	int get_mtime(char *fn, time_t &mt);
    #endif
};

#endif


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
