/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: data_input.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   data_input.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  数据输入模块
 **/

#ifndef DATA_INPUT_H
#define DATA_INPUT_H

#include "g_pack_t.h"

class DataInput{
public:
    DataInput(){}

    ~DataInput(){}

	/** 初始化 */
	int init(comcfg::ConfigUnit &conf);

	/** 启动 */
	void run(g_pack_t *g_pack)
	{
		int ret = pthread_create(&m_thread, NULL, m_thread_func, (void *)g_pack);
        if(ret != 0){
            suicide();
        }
	}

	/** 等待结束 */
	void join()
	{
		pthread_join(m_thread, NULL);
	}

private:
    DataInput(const DataInput&);
    DataInput& operator=(const DataInput&);

	pthread_t m_thread;     //!< 线程

	static void *m_thread_func(void *); //!< 线程函数

};


#endif
