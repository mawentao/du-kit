/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: ubsvr.h, v1.0.0 __DATETIME__, Exp $
 *
 **************************************************************************/

/**
 * @file   ubsvr.h
 * @date   __DATETIME__
 * @brief  ub_server框架实现
 **/

#ifndef ___UBSVR_H__
#define ___UBSVR_H__

#include <bsl/pool.h>
#include <nshead.h>
#include <ub_server.h>
#include <Configure.h>
#include "_NAMESPACE__func.h"

namespace _NAMESPACE_
{
    //! mcpack version
    const uint32_t SVR_MCPACK_VER = 2;

    /**
     * @brief server 框架类, 基于ub_server封装
     *
    **/
    template<
             /**
              * idl 请求体:
              *     proxy 请求中为 idl::proxy_req_t
              *     storage 请求中为 idl::storage_req_t
             **/
             class _IdlReq,
             /**
              * idl 响应体:
              *     proxy 请求中为 idl::proxy_res_t
              *     storage 请求中为 idl::storage_res_t
             **/
             class _IdlRes,
             /**
              * idl 使用的mempool类型, 默认为syspool
             **/
             class _IdlPool = bsl::xcompool >
    class Server
    {
    public:
        typedef _IdlReq idl_req_t;
        typedef _IdlRes idl_res_t;
        typedef _IdlPool idl_pool_t;
        typedef Server<idl_req_t, idl_res_t, idl_pool_t > _Self;

        /**
         * @brief server回调函数原型
         * @param [in]     nshead     : 请求nshead
         * @param [in]     req        : 请求idl结构体
         * @param [out]    res        : 响应idl结构体
         * @param [in/out] user_data  : 请求nshead + idl_req_t
         * @param [in/out] pool       : idl mempool
         * @return  int 是否出错
         * @retval  _NAMESPACE_::SUCC:ok _NAMESPACE_::FAIL:出错,关闭连接
        **/
        typedef int (*callback_func_t)(const nshead_t &nshead,
                                       const idl_req_t &req,
                                       idl_res_t &res,
                                       void *user_data,
                                       idl_pool_t& pool);

        /**
         * @brief 异常处理回调, ub回调函数中idl序列化到mcpack抛异常时调用
         *        主要工作: 记录log, 添加sf_ams监控,
         *                  在检索请求中还将酌情删除尾部记录
         *
         * @param [in]     e       : 抛出的bsl::Exception
         * @param [in/out] res     : idl 响应体
         * @param [in]     on_save : 是否idl.save动作抛出的异常, false:no,true:yes
         *
         * @return int 是否重试异常操作 0:不重试 1:重试
         * @date 2010/06/12 10:39:09
        **/
        typedef int (*exception_handle_t)(const bsl::Exception &e,
                                          idl_res_t *res,
                                          bool on_save);

        /**
         * @brief 线程共享缓冲区初始化函数
         *
         * @param [in]     ithrd    : 线程号 (0 - thrad_num)
         * @param [in/out] user_buf : 用户线程共享缓冲区
         *
         * @return  int 是否成功 _NAMESPACE_::SUCC:成功 _NAMESPACE_::FAIL:失败
         * @date 2010/06/12 11:09:34
        **/
        typedef int (*shared_init_t)(uint32_t ithrd, void* shared_buf);

        //! 线程共享缓冲区清除函数, 释放内部缓冲区, 不释放指针本身
        typedef shared_init_t shared_recycle_t;

    public:

        /**
         * @brief 构造函数
        **/
        Server():
            _inited(false), _ub_server(NULL), _callback(NULL),
            _shared_recycle(NULL), _exception_handle(NULL),
            _arr_shared(NULL)
        {// empty
        }

        /**
         * @brief de-ctor, 释放Ctor中分配的资源
        **/
        virtual ~Server();

        /**
         * @brief 初始化服务配置
         *
         * @param [in] svr_conf        : server配置单元,包含ip/端口/线程数/读写buf大小等信息
         * @param [in] shared_size     : 线程共享空间大小
         * @param [in] shared_init     : 线程共享空间初始化函数
         * @param [in] exception_handle: 异常回调
         *
         * @return int always return _NAMESPACE_::SUCC, 失败直接退出进程
        **/
        int init(const comcfg::ConfigUnit& svr_conf,
                 const uint32_t shared_size,
                 shared_init_t shared_init,
                 shared_recycle_t shared_recycle,
                 exception_handle_t exception_handle);

        /**
         * @brief 启动server
         *
         * @param [in] callback   : server回调函数
         * @return  int 是否成功 _NAMESPACE_::SUCC:成功 _NAMESPACE_::FAIL:失败
        **/
        int run(callback_func_t callback);

        /**
         * @brief join ub server threads
        **/
        void join();

        /**
         * @brief stop ub server
        **/
        void stop();

    private:

        /**
         * 供ub_server 使用的线程共享缓冲区
         * 均在Server初始化时填充
        **/
        struct _shared_wrapper_t
        {
            void *user_data; //!< 用户使用的线程共享缓冲区,用户自行解释

            _Self *svr_handle; //!< 线程对应的Server实例
            idl_pool_t *pool; //!< idl使用的mempool

            //rcpool_t *rcpool; //!< mcpack使用的resoucePool

            char *mcpack_tmp; //!< mcpack 使用的 tmp buffer
            uint32_t mcpack_tmp_size; //!< mcpack_tmp 大小(bytes);

        };

        /**
         * @brief 初始化_ub_svr_conf, ub_server配置项
         * @param [in] cgf_unit   : 服务对应配置单元
         * @return  int 是否成功 _NAMESPACE_::SUCC:成功 _NAMESPACE_::FAIL:失败
        **/
        int _init_svr_conf(const comcfg::ConfigUnit& cgf_unit);

        /**
         * @brief 初始化_arr_shared用户线程共享缓冲区及清理函数
         * @param [in] shared_size    : 用户buf大小
         * @param [in] shared_init    : 初始化用户buf函数
         * @param [in] shared_recycle : 清理用户buf函数
         * @return  int 是否成功 _NAMESPACE_::SUCC:成功 _NAMESPACE_::FAIL:失败
        **/
        int _init_shared_buf(const comcfg::ConfigUnit &svr_conf,
                             const uint32_t shared_size,
                             shared_init_t shared_init,
                             shared_recycle_t shared_recycle);

        /**
         * @brief ub 回调函数
         *
         * @return  int _NAMESPACE_::SUCC:成功 _NAMESPACE_::FAIL:出错、关闭连接
        **/
        static int _callback_wrapper();

        /**
         * @brief 从mcpack 缓冲区中load idl请求体, 对外屏蔽mcpack操作
         *
         * @param [in]  mcpack_buf      : mcpack buffer
         * @param [in]  mcpack_buf_size : size of mcpack buffer
         * @param [in]  rcpool          : mcpack使用的resource pool
         * @param [out] req             : idl 请求体
         * @return  int 是否成功 _NAMESPACE_::SUCC:成功 _NAMESPACE_::FAIL:失败
        **/
        static int _load_idl_req(char *mcpack_buf,
                                 uint32_t mcpack_buf_size,
                                 _shared_wrapper_t *shared_data,
                                 idl_req_t &req);

        /**
         * @brief 将idl 响应体序列化到mcpack中
         *        内部调用异常回调, 处理buffer不够情况
         * @param [in]  res        : idl 响应体
         * @param [in]  shared_data: 线程共享空间
         * @param [out] mcpack_buf : mcpack buffer
         * @param [out] nshead_res : 响应nshead
         * @return  int 是否成功 _NAMESPACE_::SUCC:成功 _NAMESPACE_::FAIL:失败
        **/
        static int _save_idl_res(idl_res_t &res,
                                 _shared_wrapper_t *shared_data,
                                 char *mcpack_buf,
                                 nshead_t &nshead_res);

        //!< disable '='
        _Self& operator = (const _Self& );
        //!< disable copy C-tor
        Server(const _Self& );

    protected:

        bool _inited;     //!< 是否已初始化
        ub_server_t *_ub_server;      //!< ub_server 实例
        ub_svr_t _ub_svr_conf;    //!< ub server 配置项
        callback_func_t _callback;    //!< 用户回调
        shared_recycle_t _shared_recycle;     //!< 用户缓冲区情理函数
        exception_handle_t _exception_handle;     //!< 异常处理回调


        //!< 线程共享缓冲区,构造函数中分配、析构中释放
        void **_arr_shared;
    };

}

#include "ubsvr_impl.h"

#endif
