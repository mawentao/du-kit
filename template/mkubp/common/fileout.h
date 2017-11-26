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
 * @brief ��ȡ��ǰʱ����ַ���
 * @param[out] tbuf : char *
 * @param[in] size : size_t
 * @param[in] format : const char *
 * @return void
 */
void now_str_time(char *tbuf, size_t size, const char *format="%Y-%m-%d %H:%M:%S");


/** ��ʱд�ļ� */
class FileOut{
public:
	struct file_out_t{
		char file_prefix[1024];		//!< �ļ���ǰ׺
		char **buf;					//!< д���ļ�������
		uint32_t bufcount;			//!< �����������С
		uint32_t bufsize;			//!< ������¼�Ĵ�С
		uint32_t size;				//!< ��ǰ�������еļ�¼����
		uint32_t write_interval_s;	//!< ����д�ļ������ʱ����
		pthread_mutex_t buflock;    //!< �����Ի��������м���
	};

	/** ���캯�� */
    FileOut(){}

	/**
	 * @brief ��ʼ���ļ������
	 * @param fname_prefix : const char * �ļ���ǰ׺
	 * @param bufcount : uint32_t �����������С
	 * @param bufsize : uint32_t ������¼�Ĵ�С
	 * @param write_interval_s : uint32_t ����д�ļ������ʱ����
	 * @return int 0-�ɹ� -1:ʧ��
	 */
	int init(const char *fname_prefix, uint32_t bufcount = 100, uint32_t bufsize = 1024, uint32_t write_interval_s = 1);

	/**
     * @brief д�뻺����
	 * @param msg : const char * д�뻺����������
     **/
	void writebuf(const char *msg);

	/**
     * @brief ������ʱд�߳�
     * @return int (ͬpthread_create��������ֵ)
     **/
	int run(){
		return pthread_create(&m_pt, NULL, m_callback_fun, (void*)&m_fo);
	}

    /**
      * @brief: �ȴ��߳̽���
      * @return: int (ͬpthread_join����ֵ)
      */
    int join(){
        return pthread_join(m_pt, NULL);
    }

	/** @brief ���������ͷ���Դ */
    ~FileOut();

private:
	file_out_t m_fo;
	pthread_t  m_pt;   //!< ��ʱд�߳�

	static void append_to_file(file_out_t &fo);	//!< д���ļ�
	static void *m_callback_fun(void *);        //! ����߳�ʹ�õĻص�����
};


#endif
