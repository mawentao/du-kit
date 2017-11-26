/****************************************************************\
* @Copyright (C) 2011 Baidu.com, Inc. All Rights Reserved 
* @File server_impl.h
* @Author wangwf
* @Email wangweifeng@baidu.com
* @Date 2011/11/04 22:03:57
* @Version 1.0
* @Brief server框架具体实现(from pvstat-yangbaokui)
*  
\****************************************************************/
#ifndef  __SERVER_IMPL_H_
#define  __SERVER_IMPL_H_

#include <mc_pack.h>
#include <bsl/exception.h>

#include "server.h"

namespace icat
{

    template<class _IdlReq, class _IdlRes, class _IdlPool>
    int Server<_IdlReq, _IdlRes, _IdlPool>::_init_svr_conf(
                                            const comcfg::ConfigUnit &cfg_unit)
    {/*{{{*/
        try
        {
            memset(&_ub_svr_conf, 0, sizeof(_ub_svr_conf));
            snprintf(_ub_svr_conf.svr_name, sizeof(_ub_svr_conf.svr_name), 
                        "%s", cfg_unit["name"].to_cstr());
            _ub_svr_conf.port = cfg_unit["port"].to_uint32();
            _ub_svr_conf.read_timeout = cfg_unit["read_timeout"].to_uint32();
            _ub_svr_conf.write_timeout = cfg_unit["write_timeout"].to_uint32();
            _ub_svr_conf.thread_num = cfg_unit["thread_num"].to_uint32();
            _ub_svr_conf.connect_type = cfg_unit["connect_type"].to_uint32();
            _ub_svr_conf.server_type = cfg_unit["server_type"].to_uint32();
            _ub_svr_conf.connect_timeout = cfg_unit["connect_timeout"].to_uint32();
            _ub_svr_conf.read_buf_size = cfg_unit["read_buf_size"].to_uint32();
            _ub_svr_conf.write_buf_size = cfg_unit["write_buf_size"].to_uint32();
            UB_LOG_TRACE("init server. [name='%s' port=%u rto=%u wto=%u thead_num=%u "
                        "connect_type:%u server_type:%u cto:%u rbsize:%u wbsize:%u]",
                         _ub_svr_conf.svr_name, _ub_svr_conf.port, _ub_svr_conf.read_timeout,
                         _ub_svr_conf.write_timeout, _ub_svr_conf.thread_num,
                         _ub_svr_conf.connect_type, _ub_svr_conf.server_type, 
                         _ub_svr_conf.connect_timeout, _ub_svr_conf.read_buf_size,
                         _ub_svr_conf.write_buf_size);
        } 
        catch (bsl::Exception &e)
        {
            UB_LOG_FATAL("exception when init ub_svr conf: %s [%s:%d][%s]\n%s",
                       e.what(), e.file(), e.line(), e.name(), e.stack()); 
            return icat::FAIL;
        }
        UB_LOG_TRACE("init svr conf succ. [svr:'%s' port:%u]",
                        _ub_svr_conf.svr_name, _ub_svr_conf.port);
        return icat::SUCC;
    }/*}}}*/

    template<class _IdlReq, class _IdlRes, class _IdlPool>
    int Server<_IdlReq, _IdlRes, _IdlPool>::_init_shared_buf(
                                            const comcfg::ConfigUnit &svr_conf,
                                            const uint32_t shared_size,
                                            shared_init_t shared_init,
                                            shared_recycle_t shared_recycle)
    {/*{{{*/
        _arr_shared = (void**)calloc(_ub_svr_conf.thread_num, sizeof(void*));
        if( !_arr_shared)
        {
            UB_LOG_FATAL("calloc _arr_shared failed. [thread_num=%u]", _ub_svr_conf.thread_num);
            suicide();
        }
        for(uint32_t ithrd = 0; ithrd < _ub_svr_conf.thread_num; ithrd++)
        {
            _arr_shared[ithrd] = calloc(1, sizeof(_shared_wrapper_t));
            if( !_arr_shared[ithrd])
            {
                UB_LOG_FATAL("calloc _arr_shared[ithrd] failed. [thread_num=%u ithrd=%u]",
                                _ub_svr_conf.thread_num, ithrd);
                suicide();
            }
            _shared_wrapper_t* shared_buf = (_shared_wrapper_t*)(_arr_shared[ithrd]);
            shared_buf->svr_handle = this; 
            try
            {
                shared_buf->pool = new idl_pool_t;
                shared_buf->mcpack_tmp = (char *)malloc(svr_conf["mcpack_tmp_size"].to_uint32());
                if(!shared_buf->mcpack_tmp)
                {
                    UB_LOG_FATAL("malloc mcpack tmp buf failed. [size=%u]", 
                                    svr_conf["mcpack_tmp_size"].to_uint32());
                    suicide();
                }
                shared_buf->mcpack_tmp_size = svr_conf["mcpack_tmp_size"].to_uint32();
            } 
            catch (...)
            {
                UB_LOG_FATAL("new idl_pool or malloc mcpack tmp buf failed. "
                            "[thread_num=%u ithrd=%u]", _ub_svr_conf.thread_num, ithrd);
                suicide();
            }
            if(0 != shared_buf->pool->create())
            {
                UB_LOG_FATAL("create mempool failed. [thread_num=%u ithrd=%u]", 
                                    _ub_svr_conf.thread_num, ithrd);
                suicide();
            }
            shared_buf->user_data = calloc(shared_size, 1);
            if(!shared_buf->user_data)
            {
                UB_LOG_FATAL("calloc user shared_buf failed. [shared_size=%u thread_num=%u ithrd=%u]",
                                shared_size, _ub_svr_conf.thread_num, ithrd);
                suicide();
            }
            if(shared_init && icat::SUCC != shared_init(ithrd, shared_buf->user_data))
            {
                UB_LOG_FATAL("init shared user buf failed. [thread_num=%u ithrd=%u]",
                                _ub_svr_conf.thread_num, ithrd);
                suicide();
            }
        }
        return icat::SUCC;
    }/*}}}*/

    template<class _IdlReq, class _IdlRes, class _IdlPool>
    int Server<_IdlReq, _IdlRes, _IdlPool>::init(
                                            const comcfg::ConfigUnit &cfg_unit,
                                            const uint32_t shared_size,
                                            shared_init_t shared_init,
                                            shared_recycle_t shared_recycle,
                                            exception_handle_t exception_handle)
    {/*{{{*/

        if(_inited)
        {
            UB_LOG_WARNING("Server init already!");
            return icat::SUCC;
        }

        //! 用户缓冲区清理回调
        _shared_recycle = shared_recycle;

        //! 异常处理回调
        _exception_handle = exception_handle;

        //! 初始化ub_svr配置 
        if(0 != _init_svr_conf(cfg_unit))
        {
            UB_LOG_FATAL("init svr conf failed.");
            suicide();
        }

        //! create ub_server and load conf
        if(NULL == (_ub_server = ub_server_create()))
        {
            UB_LOG_FATAL("create ub_server failed. [name='%s']", _ub_svr_conf.svr_name);
            suicide();
        }
        if(0 != ub_server_load(_ub_server, &_ub_svr_conf))
        {
            UB_LOG_FATAL("load ub_server failed. [name='%s']", _ub_svr_conf.svr_name);
            suicide();
        }

        //! set NODELAY
        if(0 != ub_server_setoptsock(_ub_server, UBSVR_NODELAY))
        {
            UB_LOG_FATAL("set ub opt NODELAY failed. [name='%s']", _ub_svr_conf.svr_name);
            suicide();
        }
                
        //! init user-shared data
        if(0 != _init_shared_buf(cfg_unit, shared_size, shared_init, shared_recycle))
        {
            UB_LOG_FATAL("init shared buf failed. [svr='%s' port='%u']",
                            _ub_svr_conf.svr_name, _ub_svr_conf.port);
            suicide();
        }
        //! set user data
        if(0 != ub_server_set_user_data(_ub_server, _arr_shared, sizeof(_shared_wrapper_t)))
        {
            UB_LOG_FATAL("ub_server set user data failed. [thread_num=%u size=%u]",
                            _ub_svr_conf.thread_num, (uint32_t)sizeof(_shared_wrapper_t));
            suicide();
        }
        //! set read/write buf
        if(0 != ub_server_set_buffer(_ub_server, 
                                NULL, _ub_svr_conf.read_buf_size + sizeof(nshead_t),
                                NULL, _ub_svr_conf.write_buf_size + sizeof(nshead_t)))
        {
            UB_LOG_FATAL("ub_server set buffer failed. [read_size=%u write_size=%u]",
                            _ub_svr_conf.read_buf_size, _ub_svr_conf.write_buf_size);
            suicide();
        }

        UB_LOG_TRACE("init svr succ. [svr_name='%s' port:%u]",
                    _ub_svr_conf.svr_name, _ub_svr_conf.port);

        _inited = true;
        return icat::SUCC;
    }/*}}}*/

    template<class _IdlReq, class _IdlRes, class _IdlPool>
    int Server<_IdlReq, _IdlRes, _IdlPool>::_load_idl_req(
                                            char *mcpack_buf,
                                            uint32_t mcpack_buf_size,
                                            _shared_wrapper_t *shared_data,
                                            idl_req_t &req)
    {/*{{{*/
        check_null(mcpack_buf, icat::FAIL);
        check_null(shared_data, icat::FAIL);

        int func_ret = icat::SUCC;
        try
        {
            // 打开mcpack
            mc_pack_t *p_mc_req = mc_pack_open_rw(mcpack_buf, mcpack_buf_size, 
                            shared_data->mcpack_tmp, shared_data->mcpack_tmp_size);
            int ret = MC_PACK_PTR_ERR(p_mc_req);
            if(ret)
            {
                throw (bsl::Exception() << BSL_EARG)
                    .pushf("failed to open req mcpack. [ret=%d info='%s']",
                            ret, mc_pack_perror(ret));
            }
            // log req body
            {
                char tmp_buf[2048];
                tmp_buf[0] = 0;
                ret = mc_pack_pack2json(p_mc_req, tmp_buf, sizeof(tmp_buf));
                if(0 == ret)
                {
                    ub_log_pushnotice("req_body", "%s", tmp_buf);
                }
                else
                {
                    ub_log_pushnotice("req_body", "%s", "(req too long)");
                }
            }
            // load idl
            req.load(p_mc_req);
        }
        catch(bsl::Exception &e)
        {
            UB_LOG_WARNING("exception when load idl from mcpack: %s [%s:%d][%s]\n%s",
                            e.what(), e.file(), e.line(), e.name(), e.stack());
            ub_log_pushnotice("exception", "{%s}", e.what());
            func_ret = icat::FAIL;
        }
        return func_ret;
    }/*}}}*/
     
    template<class _IdlReq, class _IdlRes, class _IdlPool>
    int Server<_IdlReq, _IdlRes, _IdlPool>::_save_idl_res(
                                            idl_res_t &res,
                                            _shared_wrapper_t *shared_data,
                                            char *mcpack_buf,
                                            nshead_t &res_nshead)
    {/*{{{*/
        check_null(shared_data, icat::FAIL);
        check_null(mcpack_buf,  icat::FAIL);

        uint32_t write_buf_size = shared_data->svr_handle->_ub_svr_conf.write_buf_size;
#if 0 
        mc_pack_t *p_mc_res = mc_pack_open_w(SVR_MCPACK_VER, mcpack_buf, write_buf_size,
                                    shared_data->mcpack_tmp, shared_data->mcpack_tmp_size);

        int ret = MC_PACK_PTR_ERR(p_mc_res);
        if (ret)
        {
            UB_LOG_WARNING("failed to open req mcpack. [ret=%d info='%s']",
                            ret, mc_pack_perror(ret));
            return icat::FAIL;
        }
#endif
        bool retry_save = true;
        int func_ret = icat::SUCC;
        //!! be-careful here, TODO: make-sure this works
        while(retry_save && icat::SUCC == func_ret)
        {
            retry_save = false;

            //! open mcpack
            mc_pack_t *p_mc_res = mc_pack_open_w(SVR_MCPACK_VER, 
                                                 mcpack_buf, write_buf_size, 
                                                 shared_data->mcpack_tmp, 
                                                 shared_data->mcpack_tmp_size);
            int ret = MC_PACK_PTR_ERR(p_mc_res);
            if (ret)
            {
                UB_LOG_WARNING("failed to open req mcpack. [ret=%d info='%s']",
                                ret, mc_pack_perror(ret));
                func_ret = icat::FAIL;
                break;
            }
            
            //! try save idl to mcpack
            try
            {
                res.save(p_mc_res);                                    

                char tmp_buf[2048];
                tmp_buf[0] = 0;
                ret = mc_pack_pack2json(p_mc_res, tmp_buf, sizeof(tmp_buf));
                if(0 == ret)
                {
                    ub_log_pushnotice("res_body", "%s", tmp_buf);
                }
                else
                {
                    ub_log_pushnotice("res_body", "%s", "(res too long)");
                }
            }
            catch (bsl::Exception &e)
            {
                UB_LOG_DEBUG("exception when idl save to mcpack. %s[%s:%d][%s]\n%s",
                             e.what(), e.file(), e.line(), e.name(), e.stack());

                if(shared_data->svr_handle->_exception_handle && 
                    0 != shared_data->svr_handle->_exception_handle(e, &res, true))
                {
                    // need re-save
                    retry_save = true;
                    UB_LOG_TRACE("retry idl save on exception: %s [%s:%d][%s]\n%s",
                                    e.what(), e.file(), e.line(), e.name(), e.stack());
                }
                else // 失败
                {
                    func_ret = icat::FAIL; 
                    UB_LOG_WARNING("exception when idl_res_t save to mcpack: "
                                   "%s [%s:%d][%s]\n%s",
                                   e.what(), e.file(), e.line(), e.name(), e.stack());
                }
            }
            res_nshead.body_len = mc_pack_get_size(p_mc_res);

            //! close mcpack
            ret = mc_pack_close(p_mc_res);
            if(ret)
            {
                 UB_LOG_WARNING("failed to close mcpack. [ret=%d info='%s']",
                                 ret, mc_pack_perror(ret));
                 func_ret = icat::FAIL;
            }
        }

        if(icat::SUCC != func_ret)
        {
            UB_LOG_WARNING("idl save failed. [svr_name='%s' port=%d]",
                            shared_data->svr_handle->_ub_svr_conf.svr_name, 
                            shared_data->svr_handle->_ub_svr_conf.port);
            res_nshead.body_len = 0;
        } 

        return func_ret;
    }/*}}}*/
    
    template<class _IdlReq, class _IdlRes, class _IdlPool>
    int Server<_IdlReq, _IdlRes, _IdlPool>::_callback_wrapper()
    {/*{{{*/

        //! get thread-data and check null
        nshead_t *ph_req = (nshead_t *)ub_server_get_read_buf(),
                 *ph_res = (nshead_t *)ub_server_get_write_buf();
        _shared_wrapper_t *shared_data = (_shared_wrapper_t *)ub_server_get_user_data();

        check_null(ph_req, icat::FAIL);
        check_null(ph_res, icat::FAIL);
        check_null(shared_data, icat::FAIL);

#if 0
        //! ip authenticate
        if(icat::SUCC != shared_data->svr_handle->_ip_auth.auth_ip_fd(ub_server_get_socket()))
        {
            UB_LOG_WARNING("ip authenticate failed.");  
            return icat::FAIL;
        }
#endif
        //! set log basic fields
        {
            ub_log_setbasic(UB_LOG_SVRNAME, "%s", shared_data->svr_handle->_ub_svr_conf.svr_name);
            ph_req->provider[sizeof(ph_req->provider)-1]=0; //! 异常请求case 规避
            ub_log_setbasic(UB_LOG_REQSVR, "%s", ph_req->provider);
            ub_log_setbasic(UB_LOG_CMDNO, "%u v%x", ph_req->id, ph_req->version);
        }

        //! init ph_res
        {
            snprintf(ph_res->provider, sizeof(ph_res->provider), "%s",
                    shared_data->svr_handle->_ub_svr_conf.svr_name);
            ph_res->id = ph_req->id;
            ph_res->version = ph_req->version;
            ph_res->log_id = ph_req->log_id;
            ph_res->reserved = 0;
            ph_res->magic_num = NSHEAD_MAGICNUM;
            ph_res->body_len = 0;
        }

        int session_ret = icat::SUCC;
		//! 处理、响应请求
        try
        {
            idl_req_t req(shared_data->pool);
            idl_res_t res(shared_data->pool);

            // load idl from mcpack
            if(icat::SUCC != _load_idl_req((char*)(ph_req+1), ph_req->body_len, shared_data, req))
            {
                throw (bsl::Exception() << BSL_EARG)
                        .pushf("load idl req from mcpack failed.");
            }

            // 响应由用户回调完成
            session_ret = shared_data->svr_handle->_callback(*ph_req, req, res,
                                        shared_data->user_data, *(shared_data->pool));
            if(icat::SUCC != session_ret)
            {
                throw (bsl::Exception() << BSL_EARG)
                    .pushf("user callback failed. [ret=%d]", session_ret);
            }

            // save idl to mcpack
            if(icat::SUCC != _save_idl_res(res, shared_data, (char*)(ph_res+1), *ph_res))
            {
                throw (bsl::Exception() << BSL_EARG)
                        .pushf("save idl to mcpack buffer failed.");
            }
        }
        catch (bsl::Exception &e)
        {
            UB_LOG_WARNING("exception in server callback: %s [%s:%d][%s]\n%s",
                            e.what(), e.file(), e.line(), e.name(), e.stack());
            ub_log_pushnotice("exception", "{%s}", e.what());
            if(shared_data->svr_handle->_exception_handle)
            {
                shared_data->svr_handle->_exception_handle(e, NULL, false);
            }
            ph_res->body_len = 0;
            session_ret = icat::FAIL;
        	ub_log_setbasic(UB_LOG_ERRNO, "%d ", session_ret);
		}

        return session_ret;
    }/*}}}*/
    
    template<class _IdlReq, class _IdlRes, class _IdlPool>
    Server<_IdlReq, _IdlRes, _IdlPool>::~Server()
    {/*{{{*/
        if(_ub_server)
        {
            // 内部会 stop,join,释放空间
            ub_server_destroy(_ub_server);
            _ub_server = NULL;
        }

        if(!_arr_shared)
        {
            return ;
        }
        for(uint32_t ithrd = 0; ithrd < _ub_svr_conf.thread_num; ithrd++)
        {
            if( !_arr_shared[ithrd])
            {
                continue;
            }
            _shared_wrapper_t *shared_buf = (_shared_wrapper_t *)_arr_shared[ithrd];
            if(shared_buf)
            {
                if(shared_buf->pool)
                {
                    delete shared_buf->pool;
                    shared_buf->pool = NULL;
                }
                if(shared_buf->mcpack_tmp)
                {
                    free(shared_buf->mcpack_tmp);
                    shared_buf->mcpack_tmp = NULL;
                    shared_buf->mcpack_tmp_size = 0;
                }
                if(shared_buf->user_data)
                {
                    if(_shared_recycle)
                    {
                        _shared_recycle(ithrd, shared_buf->user_data);
                    }
                    free(shared_buf->user_data);
                    shared_buf->user_data = NULL;
                }
            }
            free(_arr_shared[ithrd]);
            _arr_shared[ithrd] = NULL;
        }
        free(_arr_shared);
        _arr_shared = NULL;
        return ;
    }/*}}}*/

    template<class _IdlReq, class _IdlRes, class _IdlPool>
    int Server<_IdlReq, _IdlRes, _IdlPool>::run(callback_func_t callback)
    {/*{{{*/
        if( !_inited)
        {
            UB_LOG_WARNING("Server should be inited before run!");
            return icat::FAIL;
        }

        if( !callback)
        {
            UB_LOG_WARNING("callback is null!");
            return icat::FAIL;
        }

        _callback = callback;
        ub_server_set_callback(_ub_server, _callback_wrapper);
        if(0 != ub_server_run(_ub_server))
        {
            UB_LOG_FATAL("server fail to run.");
            suicide();
        }
        return icat::SUCC;
    }/*}}}*/

    template<class _IdlReq, class _IdlRes, class _IdlPool>
    void Server<_IdlReq, _IdlRes, _IdlPool>::join()
    {/*{{{*/
        if(_inited)
        {
            //_ip_auth.join();
            if( 0 != ub_server_join(_ub_server))
            {
                UB_LOG_FATAL("ub_server stop failed.");
                suicide();
            }
        }
        return ;
    }/*}}}*/

    template<class _IdlReq, class _IdlRes, class _IdlPool>
    void Server<_IdlReq, _IdlRes, _IdlPool>::stop()
    {/*{{{*/
        if(_inited)
        {
            //_ip_auth.stop();
            if( 0 != ub_server_stop(_ub_server))
            {
                UB_LOG_FATAL("ub_server stop failed.");
                suicide();
            }
        }
        return ;
    }/*}}}*/


}


#endif  //__SERVER_IMPL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
