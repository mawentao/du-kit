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

/** 数据块 */
struct data_block_t{
	uint64_t index; //!< 数据块序号
	char **buf;     //!< 数据块内存
	uint32_t size;  //!< 当前数据块中的数据个数
	int state;		//!< 数据块状态 [0 1 2 3 4 5]

public:
	/**
     * @brief 初始化
	 * @param[in] buf_size : 每个数据单元的内存大小
	 * @param[in] data_block_size : 数据单元的个数
	 * @return int 0-成功 1-失败
	 **/
	int init(uint32_t buf_size, uint32_t data_block_size);
};


/** 全局内存 */
struct g_pack_t{
	/***************** 命令行参数信息 **********************/
	char exe_path[FILE_NAME_LEN];	//!< 可执行程序所在目录
	char input_file[FILE_NAME_LEN]; //!< 输入文件
	char output_file[FILE_NAME_LEN];//!< 输出文件
	uint64_t pro_line_num;          //!< 需要处理的行数

	/***************** 配置信息 **********************/
	uint32_t line_buf_size;	      //!< line_buf_size
	uint32_t data_block_size;	  //!< 数据块大小
	uint32_t data_block_arr_size; //!< 数据块数组大小
	uint32_t process_thread_num;  //!< 数据处理线程的个数
	uint32_t max_col_num;		  //!< 一行分割之后的最大字段个数
	uint32_t max_col_len;		  //!< 一列字段最大长度
	bool keep_order;              //!< 是否保持顺序输出

	/***************** 线程共享内存区 **********************/

	data_block_t *block_arr;	  //!< 数据块数组
	uint32_t last_read_index;	  //!< 上次读到的数据块位置
	uint32_t last_write_index;    //!< 上次写到的数据块位置
	uint32_t last_process_index;  //!< 上次处理到的数据块位置

	bool read_finish;    //!< 全部输入完毕

	pthread_mutex_t block_lock;   //!< 用来对block_arr进行加锁

public:
	/** 获取可执行程序所在的目录  */
	void get_exe_path();

	/** 从配置文件中读取配置初始化  */
	void init(comcfg::ConfigUnit &conf);

	/** 调试用 */
	void output(FILE *fp=stdout);
};


/** 退出 */
inline void suicide()
{
	UB_LOG_FATAL("process suicided.");
	sync();
	kill(getpid(), SIGTERM);
}


/**
 * @brief 获取一个指定状态的数据块
 * @param[in] g_pack_t &
 * @param[in] block_state
 * @param[in] uint64_t : 当需要保持输出顺序一致时用此参数
 **/
data_block_t * get_block(g_pack_t &g, int block_state, uint64_t index=0);

#endif
