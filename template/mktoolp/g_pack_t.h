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

/** ȫ���ڴ� */
struct g_pack_t{
	/***************** ������Ϣ **********************/

	char exe_path[FILE_NAME_LEN];	//!< ��ִ�г�������Ŀ¼
	char input_file[FILE_NAME_LEN]; //!< �����ļ�
	char output_file[FILE_NAME_LEN];//!< ����ļ�
	uint64_t pro_line_num;          //!< ��Ҫ���������

	/***************** �ļ����������ʹ�õ��ڴ� **********************/

	char *line_buf;			//!< ��һ���ļ�����
	uint32_t line_buf_size;	//!< line_buf_size
	char **cols;			//!< �洢һ�зָ�֮���ÿ���ֶ�
	uint32_t col_num;		//!< һ�зָ�֮����ֶθ���
	uint32_t max_col_num;	//!< һ�зָ�֮�������ֶθ���
	uint32_t max_col_len;	//!< һ���ֶ���󳤶�

public:
	/** ��ȡ��ִ�г������ڵ�Ŀ¼  */
	void get_exe_path();

	/** �������ļ��ж�ȡ���ó�ʼ��  */
	void init(comcfg::ConfigUnit &conf);

	/** ��ָ���ַ��ָ��� */
	void split_line(char c);

	void output(FILE *fp=stdout){
		fprintf(fp, "g_pack: {\n");
		fprintf(fp, "    %-14s: %s\n", "exe_path", exe_path);
		fprintf(fp, "    %-14s: %s\n", "input_file", input_file);
		fprintf(fp, "    %-14s: %s\n", "output_file", output_file);
		fprintf(fp, "    %-14s: %lu\n", "pro_line_num", pro_line_num);
		fprintf(fp, "    %-14s: %lu\n", "line_buf_size", line_buf_size);
		fprintf(fp, "    %-14s: %lu\n", "max_col_num", max_col_num);
		fprintf(fp, "    %-14s: %lu\n", "max_col_len", max_col_len);
		fprintf(fp, "}\n");
	}

};




#endif
