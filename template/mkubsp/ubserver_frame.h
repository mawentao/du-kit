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
 * @brief  UBServer框架
 **/

#ifndef UBSERVER_FRAME_H
#define UBSERVER_FRAME_H

#include <bsl/pool.h>
#include <mc_pack.h>
#include "util.h"


const uint32_t SVR_MCPACK_VER = 2; //!< 当前所用的 mcpack 版本号


/**
 * @brief server 框架类
 */
class server_idl_t
{
	public:
		//! server 框架使用的内存池类型
		typedef bsl::xcompool pool_t;

		//! 用户传递的回调函数
		typedef int (*callback_func_t)(int sock, nshead_t *head, mc_pack_t *body, char *tmpbuf, size_t tmpsize);

		//! 构造函数
		server_idl_t(const ub_svr_t &svr_conf,
				size_t buf_read_size, size_t buf_write_size,
				size_t buf_mcpack_size, size_t shared_size,
				int (*shared_init)(unsigned int ithrd, void *buf),
				void (*exception_handler)(bsl::Exception &e)); // 异常处理时调用, 用于 sf_ams 增加监控信息

		//! 析构函数, 释放构造函数中分配的线程共享缓冲区
		virtual ~server_idl_t();

		//! 启动 server, 并设置回调函数为 callback
		int run(callback_func_t callback);

		//! join server thread
		void join();

		//! 获得当前线程对应的内存池对象
		static pool_t &get_pool();

		//! 获得当前线程对应的用户缓冲区
		static void *get_shared_buf();

		//! 获得 mcpack 临时缓冲区
		static char *get_mcpack_tmp_buf();

		//! 获得 mcpack 临时缓冲区的size
		size_t get_mcpack_tmp_size(){return _buf_mcpack_size;}

	protected:
		//! 供 server 框架使用的 user_buf
		//! ub 的 user_buf 前面是 _buf_head_t, 后面是用户指定的 shared_size
		//! 前三个字段在 server_idl_t 初始化时填写,
		//! 后两个字段每次进入回调函数时填写
		struct _buf_head_t
		{
			server_idl_t *parent;   //!< 指向 server_idl_t 类对象的指针
			char *mcpack_tmp_buf;   //!< 重复使用的 mcpack 临时缓冲区
			pool_t *p_pool;         //!< 每个线程一个独立的内存池
		};

		size_t _buf_read_size,
			   _buf_write_size,
			   _buf_mcpack_size;

		void (*_exception_handler)(bsl::Exception &e);
		callback_func_t _callback;  //!< 用户提供的回调函数
		const ub_svr_t _svr_conf;   //!< server 的配置, 为全局变量 g_conf 中某个 ub_svr_t 的拷贝
		auto_ub_server_t _server;   //!< server 对应的 ub_server_t * 变量
		bool _enabled;              //!< 该 server 是否启用, 根据线程数是否为 0 判断
		void **_arr_shared;         //!< 线程共享变量的存储空间, 在构造函数中分配

		/**
		 *      供 ub_server 使用的回调函数
		 *      @return 始终为 0
		 */
		static int _callback_wrapper();

		server_idl_t &operator =(const server_idl_t &); //!< disabled
};



#endif
