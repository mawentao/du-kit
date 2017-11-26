/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: ubserver_frame.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   ubserver_frame.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  UBServer���
 **/

#ifndef UBSERVER_FRAME_H
#define UBSERVER_FRAME_H

#include <bsl/pool.h>
#include <mc_pack.h>
#include "util.h"


const uint32_t SVR_MCPACK_VER = 2; //!< ��ǰ���õ� mcpack �汾��


/**
 * @brief server �����
 */
class server_idl_t
{
	public:
		//! server ���ʹ�õ��ڴ������
		typedef bsl::xcompool pool_t;

		//! �û����ݵĻص�����
		typedef int (*callback_func_t)(int sock, nshead_t *head, mc_pack_t *body, char *tmpbuf, size_t tmpsize);

		//! ���캯��
		server_idl_t(const ub_svr_t &svr_conf,
				size_t buf_read_size, size_t buf_write_size,
				size_t buf_mcpack_size, size_t shared_size,
				int (*shared_init)(unsigned int ithrd, void *buf),
				void (*exception_handler)(bsl::Exception &e)); // �쳣����ʱ����, ���� sf_ams ���Ӽ����Ϣ

		//! ��������, �ͷŹ��캯���з�����̹߳�������
		virtual ~server_idl_t();

		//! ���� server, �����ûص�����Ϊ callback
		int run(callback_func_t callback);

		//! join server thread
		void join();

		//! ��õ�ǰ�̶߳�Ӧ���ڴ�ض���
		static pool_t &get_pool();

		//! ��õ�ǰ�̶߳�Ӧ���û�������
		static void *get_shared_buf();

		//! ��� mcpack ��ʱ������
		static char *get_mcpack_tmp_buf();

		//! ��� mcpack ��ʱ��������size
		size_t get_mcpack_tmp_size(){return _buf_mcpack_size;}

	protected:
		//! �� server ���ʹ�õ� user_buf
		//! ub �� user_buf ǰ���� _buf_head_t, �������û�ָ���� shared_size
		//! ǰ�����ֶ��� server_idl_t ��ʼ��ʱ��д,
		//! �������ֶ�ÿ�ν���ص�����ʱ��д
		struct _buf_head_t
		{
			server_idl_t *parent;   //!< ָ�� server_idl_t ������ָ��
			char *mcpack_tmp_buf;   //!< �ظ�ʹ�õ� mcpack ��ʱ������
			pool_t *p_pool;         //!< ÿ���߳�һ���������ڴ��
		};

		size_t _buf_read_size,
			   _buf_write_size,
			   _buf_mcpack_size;

		void (*_exception_handler)(bsl::Exception &e);
		callback_func_t _callback;  //!< �û��ṩ�Ļص�����
		const ub_svr_t _svr_conf;   //!< server ������, Ϊȫ�ֱ��� g_conf ��ĳ�� ub_svr_t �Ŀ���
		auto_ub_server_t _server;   //!< server ��Ӧ�� ub_server_t * ����
		bool _enabled;              //!< �� server �Ƿ�����, �����߳����Ƿ�Ϊ 0 �ж�
		void **_arr_shared;         //!< �̹߳�������Ĵ洢�ռ�, �ڹ��캯���з���

		/**
		 *      �� ub_server ʹ�õĻص�����
		 *      @return ʼ��Ϊ 0
		 */
		static int _callback_wrapper();

		server_idl_t &operator =(const server_idl_t &); //!< disabled
};



#endif
