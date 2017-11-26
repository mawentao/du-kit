/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: g_pack_t.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   g_pack_t.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  ȫ�ֽṹ�嶨��
 **/

#ifndef G_PACK_T_H
#define G_PACK_T_H

#include "bsl/string.h"
#include "bsl/containers/hash/bsl_hashmap.h"
#include "Configure.h"
#include "ub.h"
#include "util.h"

#define FILE_NAME_LEN 1024     //!< �ļ�������󳤶�
#define MAX_COL_LEN 1024	   //!< ÿ����󳤶�

/** ���ݿ� */
struct data_block_t{
	uint64_t index; //!< ���ݿ����
	char **buf;     //!< ���ݿ��ڴ�
	uint32_t size;  //!< ��ǰ���ݿ��е����ݸ���
	int state;		//!< ���ݿ�״̬ [0 1 2 3 4 5]

public:
	/**
     * @brief ��ʼ��
	 * @param[in] buf_size : ÿ�����ݵ�Ԫ���ڴ��С
	 * @param[in] data_block_size : ���ݵ�Ԫ�ĸ���
	 * @return int 0-�ɹ� 1-ʧ��
	 **/
	int init(uint32_t buf_size, uint32_t data_block_size);
};


/** ȫ���ڴ� */
struct g_pack_t{
	/***************** �����в�����Ϣ **********************/
	char exe_path[FILE_NAME_LEN];	//!< ��ִ�г�������Ŀ¼
	char input_file[FILE_NAME_LEN]; //!< �����ļ�
	char output_file[FILE_NAME_LEN];//!< ����ļ�
	uint64_t pro_line_num;          //!< ��Ҫ���������

	/***************** ������Ϣ **********************/
	uint32_t line_buf_size;	      //!< line_buf_size
	uint32_t data_block_size;	  //!< ���ݿ��С
	uint32_t data_block_arr_size; //!< ���ݿ������С
	uint32_t process_thread_num;  //!< ���ݴ����̵߳ĸ���
	uint32_t max_col_num;		  //!< һ�зָ�֮�������ֶθ���
	uint32_t max_col_len;		  //!< һ���ֶ���󳤶�
	bool keep_order;              //!< �Ƿ񱣳�˳�����

	/***************** �̹߳����ڴ��� **********************/

	data_block_t *block_arr;	  //!< ���ݿ�����
	uint32_t last_read_index;	  //!< �ϴζ��������ݿ�λ��
	uint32_t last_write_index;    //!< �ϴ�д�������ݿ�λ��
	uint32_t last_process_index;  //!< �ϴδ��������ݿ�λ��

	bool read_finish;    //!< ȫ���������

	pthread_mutex_t block_lock;   //!< ������block_arr���м���

public:
	/** ��ȡ��ִ�г������ڵ�Ŀ¼  */
	void get_exe_path();

	/** �������ļ��ж�ȡ���ó�ʼ��  */
	void init(comcfg::ConfigUnit &conf);

	/** ������ */
	void output(FILE *fp=stdout);
};


/** �˳� */
inline void suicide()
{
	UB_LOG_FATAL("process suicided.");
	sync();
	kill(getpid(), SIGTERM);
}


/**
 * @brief ��ȡһ��ָ��״̬�����ݿ�
 * @param[in] g_pack_t &
 * @param[in] block_state
 * @param[in] uint64_t : ����Ҫ�������˳��һ��ʱ�ô˲���
 **/
data_block_t * get_block(g_pack_t &g, int block_state, uint64_t index=0);

#endif
