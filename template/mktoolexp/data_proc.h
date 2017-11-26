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
 * @brief  ���ݴ���ģ��
 **/

#ifndef DATA_PROC_H
#define DATA_PROC_H

#include "g_pack_t.h"
#include "nqc_client.h"


class DataProc{
public:
	struct data_proc_pack_t{
		ubclient_conf_t ubclient_conf;   //!< ubclient_conf
		g_pack_t *g_pack;  //!< ȫ���ڴ�ռ�
	};

public:
    DataProc(){}

    ~DataProc(){}

	/** ��ʼ�� */
	int init(comcfg::ConfigUnit &conf);

	/** ���� */
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

	/** �ȴ����� */
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
	pthread_t *m_threads;    //!< �߳�
	uint32_t m_thread_num;   //!< �̸߳���

	static void *m_thread_func(void *); //!< �̺߳���
};


/** ���ݴ����̵߳Ķ����ڴ� */
struct t_pack_t{
	char **cols;	      //!< �洢һ�зָ�֮���ÿ���ֶ�
	uint32_t col_num;     //!< һ�зָ�֮����ֶθ���
	uint32_t max_col_num; //!< һ�зָ�֮�������ֶθ���
	uint32_t max_col_len; //!< һ���ֶ���󳤶�

public:
	t_pack_t(uint32_t max_colnum, uint32_t max_collen);

	~t_pack_t();

	/** ��ָ���ַ��ָ��� */
	void split_line(const char *line, char c='\t');
};


#endif
