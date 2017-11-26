/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: ubserver_frame.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   ubserver_frame.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  ubserver_frame.cpp
 **/

#include "ubserver_frame.h"
#include "g_data.h"

int server_idl_t::_callback_wrapper()
{/*{{{*/
    //回调函数预处理：print log && get head and mcpack

    nshead_t *ph_req = (nshead_t *)ub_server_get_read_buf();
    void *user_data = ub_server_get_user_data();
    check_null(ph_req, FUN_FAIL);
    check_null(user_data, FUN_FAIL);

    _buf_head_t *_buf_head = (_buf_head_t *)user_data;
     mc_pack_t *p_mc_req = mc_pack_open_rw((char *)(ph_req + 1), ph_req->body_len,
                _buf_head->mcpack_tmp_buf, _buf_head->parent->_buf_mcpack_size);

    int sock = ub_server_get_socket();
    return _buf_head->parent->_callback(sock, ph_req, p_mc_req,
                                 _buf_head->mcpack_tmp_buf,
                                 _buf_head->parent->_buf_mcpack_size);

}/*}}}*/

server_idl_t::server_idl_t(const ub_svr_t &svr_conf,
        size_t buf_read_size, size_t buf_write_size,
        size_t buf_mcpack_size, size_t shared_size,
        int (*shared_init)(unsigned int ithrd, void *buf),
        void (*exception_handler)(bsl::Exception &e))
    :_buf_read_size(buf_read_size), _buf_write_size(buf_write_size),
    _buf_mcpack_size(buf_mcpack_size), _exception_handler(exception_handler), _callback(NULL),
    _svr_conf(svr_conf), _enabled(svr_conf.thread_num > 0), _arr_shared(NULL)
{/*{{{*/
    if (!_enabled)
    {
        return;
    }

    _server() = ub_server_create();
    if (!_server())
    {
        UB_LOG_FATAL("failed to create server. [name='%s']", svr_conf.svr_name);
        suicide();
    }
    if (ub_server_load(_server(), (ub_svr_t *)&svr_conf))
    {
        UB_LOG_FATAL("load server failed. [name='%s']", svr_conf.svr_name);
        suicide();
    }
    ub_server_setoptsock(_server(), UBSVR_NODELAY);

    // 创建用户缓冲区 (thread shared data), 在不同会话间共享
    _arr_shared = (void **)calloc(_svr_conf.thread_num, sizeof(void *));
    if (!_arr_shared)
    {
        UB_LOG_FATAL("calloc _arr_shared failed. [thread_num=%u]", _svr_conf.thread_num);
        suicide();
    }
    for (unsigned int ithrd = 0; ithrd < _svr_conf.thread_num; ++ithrd)
    {
        _arr_shared[ithrd] = calloc(1, sizeof(_buf_head_t) + shared_size);
        if (!_arr_shared[ithrd])
        {
            UB_LOG_FATAL("calloc _arr_shared[ithrd] failed. [thread_num=%u ithrd=%u]", 
                    _svr_conf.thread_num, ithrd);
            suicide();
        }
        _buf_head_t *p_buf_head = (_buf_head_t *)_arr_shared[ithrd];
        // 初始化 buf head
        p_buf_head->parent = this;
        p_buf_head->mcpack_tmp_buf = (char *)calloc(_buf_mcpack_size, 1);
        if (!p_buf_head->mcpack_tmp_buf)
        {
            UB_LOG_FATAL("calloc mcpack tmp buf failed. [thread_num=%u ithrd=%u]", 
                    _svr_conf.thread_num, ithrd);
            suicide();
        }
        try
        {
            p_buf_head->p_pool = new pool_t();
        }
        catch (...)
        {
            UB_LOG_FATAL("failed to new mem pool. [thread_num=%u ithrd=%u]", 
                    _svr_conf.thread_num, ithrd);
            suicide();
        }

        if (0 != p_buf_head->p_pool->create())
        {
            UB_LOG_FATAL("failed to create mem pool. [thread_num=%u ithrd=%u]", 
                    _svr_conf.thread_num, ithrd);
            suicide();
        }

        // 用户空间
        void *p_shared = p_buf_head + 1;
        if (shared_init)
        {
            if (shared_init(ithrd, p_shared) == FUN_FAIL)
            {
                UB_LOG_FATAL("init shared data failed. [thread=%u]", ithrd);
                suicide();
            }
        }
    }

    if (ub_server_set_user_data(_server(), _arr_shared, sizeof(_buf_head_t) + shared_size) != 0)
    {
        UB_LOG_FATAL("set user data failed. [size=%lu]", sizeof(_buf_head_t) + shared_size);
        suicide();
    }
    if (ub_server_set_buffer(_server(),
                NULL, _buf_read_size + sizeof(nshead_t), 
                NULL, _buf_write_size + sizeof(nshead_t)
                ) != 0)
    {
        UB_LOG_FATAL("set buffer failed. [read_size=%lu write_size=%lu]",
                _buf_read_size, _buf_write_size);
        suicide();
    }
}/*}}}*/

server_idl_t::~server_idl_t()
{/*{{{*/
    if (!_arr_shared)
    {
        return;
    }
    for (uint32_t ithrd = 0; ithrd < _svr_conf.thread_num; ++ithrd)
    {
        if (!_arr_shared[ithrd])
        {
            continue;
        }
        _buf_head_t *p_buf_head = (_buf_head_t *)_arr_shared[ithrd];
        if (p_buf_head)
        {
            if (p_buf_head->mcpack_tmp_buf)
            {
                free(p_buf_head->mcpack_tmp_buf);
                p_buf_head->mcpack_tmp_buf = NULL;
            }
            if (p_buf_head->p_pool)
            {
                delete p_buf_head->p_pool;
                p_buf_head->p_pool = NULL;
            }
        }
        free(_arr_shared[ithrd]);
        _arr_shared[ithrd] = NULL;
    }
    free(_arr_shared);
    _arr_shared = NULL;
}/*}}}*/

int server_idl_t::run(callback_func_t callback)
{/*{{{*/
    if (!_enabled)
    {
        return FUN_SUCC;
    }
    if (!callback)
    {
        UB_LOG_WARNING("null callback.");
        return FUN_FAIL;
    }

    _callback = callback;
    ub_server_set_callback(_server(), _callback_wrapper);
//    ub_server_set_callback(_server(), _callback);
    if (ub_server_run(_server()) != 0)
    {
        UB_LOG_FATAL("server failed to run.");
        suicide();
    }
    return FUN_SUCC;
}/*}}}*/

void server_idl_t::join()
{/*{{{*/
    if (_enabled)
    {
        ub_server_join(_server());
    }
}/*}}}*/

server_idl_t::pool_t &server_idl_t::get_pool()
{/*{{{*/
    _buf_head_t *p_buf_head = (_buf_head_t *)ub_server_get_user_data();
    return *(p_buf_head->p_pool);
}/*}}}*/

void *server_idl_t::get_shared_buf()
{/*{{{*/
    _buf_head_t *p_buf_head = (_buf_head_t *)ub_server_get_user_data();
    return (void *)(p_buf_head + 1);
}/*}}}*/

char *server_idl_t::get_mcpack_tmp_buf()
{/*{{{*/
    _buf_head_t *p_buf_head = (_buf_head_t *)ub_server_get_user_data();
    return p_buf_head->mcpack_tmp_buf;
}/*}}}*/



// vim:fdm=marker:nu:ts=4:sw=4:expandtab
