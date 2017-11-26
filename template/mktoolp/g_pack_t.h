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
 * @brief  全局结构体定义
 **/

#ifndef G_PACK_T_H
#define G_PACK_T_H

#include "bsl/string.h"
#include "bsl/containers/hash/bsl_hashmap.h"
#include "Configure.h"
#include "ub.h"
#include "util.h"

#define FILE_NAME_LEN 1024     //!< 文件名的最大长度
#define MAX_COL_LEN 1024	   //!< 每列最大长度

/** 全局内存 */
struct g_pack_t{
	/***************** 配置信息 **********************/

	char exe_path[FILE_NAME_LEN];	//!< 可执行程序所在目录
	char input_file[FILE_NAME_LEN]; //!< 输入文件
	char output_file[FILE_NAME_LEN];//!< 输出文件
	uint64_t pro_line_num;          //!< 需要处理的行数

	/***************** 文件处理过程中使用的内存 **********************/

	char *line_buf;			//!< 存一行文件数据
	uint32_t line_buf_size;	//!< line_buf_size
	char **cols;			//!< 存储一行分割之后的每个字段
	uint32_t col_num;		//!< 一行分割之后的字段个数
	uint32_t max_col_num;	//!< 一行分割之后的最大字段个数
	uint32_t max_col_len;	//!< 一列字段最大长度

public:
	/** 获取可执行程序所在的目录  */
	void get_exe_path();

	/** 从配置文件中读取配置初始化  */
	void init(comcfg::ConfigUnit &conf);

	/** 按指定字符分割行 */
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
