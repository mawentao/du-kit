/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: data_proc.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   data_proc.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  数据处理模块
 **/

#ifndef DATA_PROC_H
#define DATA_PROC_H

#include "g_pack_t.h"
#include "nqc_client.h"


class DataProc{
public:
	struct data_proc_pack_t{
		ubclient_conf_t ubclient_conf;   //!< ubclient_conf
		g_pack_t *g_pack;  //!< 全局内存空间
	};

public:
    DataProc(){}

    ~DataProc(){}

	/** 初始化 */
	int init(comcfg::ConfigUnit &conf);

	/** 启动 */
	void run(g_pack_t *g_pack)
	{
		int ret = 0;
		m_pack.g_pack = g_pack;
		for(uint32_t i=0; i<m_thread_num; ++i){
			ret = pthread_create(&m_threads[i], NULL, m_thread_func, (void*)&m_pack);
			if(0 != ret){
				UB_LOG_FATAL("run p_data_process_fun fail.");
				suicide();
			}
		}
	}

	/** 等待结束 */
	void join()
	{
		for(uint32_t i=0; i<m_thread_num; ++i){
			pthread_join(m_threads[i], NULL);
		}
	}

private:
    DataProc(const DataProc&);
    DataProc& operator=(const DataProc&);

	data_proc_pack_t m_pack; //!< m_pack
	pthread_t *m_threads;    //!< 线程
	uint32_t m_thread_num;   //!< 线程个数

	static void *m_thread_func(void *); //!< 线程函数
};


/** 数据处理线程的独立内存 */
struct t_pack_t{
	char **cols;	      //!< 存储一行分割之后的每个字段
	uint32_t col_num;     //!< 一行分割之后的字段个数
	uint32_t max_col_num; //!< 一行分割之后的最大字段个数
	uint32_t max_col_len; //!< 一列字段最大长度

public:
	t_pack_t(uint32_t max_colnum, uint32_t max_collen);

	~t_pack_t();

	/** 按指定字符分割行 */
	void split_line(const char *line, char c='\t');
};


#endif
