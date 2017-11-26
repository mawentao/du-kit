/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: data_output.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   data_output.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  �������ģ��
 **/

#ifndef DATA_OUTPUT_H
#define DATA_OUTPUT_H

#include "g_pack_t.h"

class DataOutput{
public:
    DataOutput(){}

    ~DataOutput(){}

	/** ��ʼ�� */
	int init(comcfg::ConfigUnit &conf);

	/** ���� */
	void run(g_pack_t *g_pack)
	{
		int ret = pthread_create(&m_thread, NULL, m_thread_func, (void *)g_pack);
        if(ret != 0){
            suicide();
        }
	}

	/** �ȴ����� */
	void join()
	{
		pthread_join(m_thread, NULL);
	}

private:
    DataOutput(const DataOutput&);
    DataOutput& operator=(const DataOutput&);

	pthread_t m_thread;     //!< �߳�

	static void *m_thread_func(void *); //!< �̺߳���

};


#endif
