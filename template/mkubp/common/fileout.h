/***************************************************************************
 *
 * Copyright (c) 2012 Baidu.com, Inc. All Rights Reserved
 * $Id: fileout.h, v1.0.0, 2012-09-15 09:14:36, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   fileout.h
 * @author mawt(mawentao@baidu.com)
 * @date   2012-09-15 09:14:36
 * @brief  fileout.h
 **/

#ifndef __FILEOUT_H__
#define __FILEOUT_H__

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


/**
 * @brief 获取当前时间的字符串
 * @param[out] tbuf : char *
 * @param[in] size : size_t
 * @param[in] format : const char *
 * @return void
 */
void now_str_time(char *tbuf, size_t size, const char *format="%Y-%m-%d %H:%M:%S");


/** 定时写文件 */
class FileOut{
public:
	struct file_out_t{
		char file_prefix[1024];		//!< 文件名前缀
		char **buf;					//!< 写入文件缓冲区
		uint32_t bufcount;			//!< 缓冲区数组大小
		uint32_t bufsize;			//!< 单条记录的大小
		uint32_t size;				//!< 当前缓冲区中的记录个数
		uint32_t write_interval_s;	//!< 两次写文件的最大时间间隔
		pthread_mutex_t buflock;    //!< 用来对缓冲区进行加锁
	};

	/** 构造函数 */
    FileOut(){}

	/**
	 * @brief 初始化文件监控类
	 * @param fname_prefix : const char * 文件名前缀
	 * @param bufcount : uint32_t 缓冲区数组大小
	 * @param bufsize : uint32_t 单条记录的大小
	 * @param write_interval_s : uint32_t 两次写文件的最大时间间隔
	 * @return int 0-成功 -1:失败
	 */
	int init(const char *fname_prefix, uint32_t bufcount = 100, uint32_t bufsize = 1024, uint32_t write_interval_s = 1);

	/**
     * @brief 写入缓冲区
	 * @param msg : const char * 写入缓冲区的数据
     **/
	void writebuf(const char *msg);

	/**
     * @brief 启动定时写线程
     * @return int (同pthread_create函数返回值)
     **/
	int run(){
		return pthread_create(&m_pt, NULL, m_callback_fun, (void*)&m_fo);
	}

    /**
      * @brief: 等待线程结束
      * @return: int (同pthread_join返回值)
      */
    int join(){
        return pthread_join(m_pt, NULL);
    }

	/** @brief 析构函数释放资源 */
    ~FileOut();

private:
	file_out_t m_fo;
	pthread_t  m_pt;   //!< 定时写线程

	static void append_to_file(file_out_t &fo);	//!< 写入文件
	static void *m_callback_fun(void *);        //! 监控线程使用的回调函数
};


#endif
