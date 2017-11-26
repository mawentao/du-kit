/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: ubsvr.h, v1.0.0 __DATETIME__, Exp $
 *
 **************************************************************************/

/**
 * @file   ubsvr.h
 * @date   __DATETIME__
 * @brief  ub_server���ʵ��
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
     * @brief server �����, ����ub_server��װ
     *
    **/
    template<
             /**
              * idl ������:
              *     proxy ������Ϊ idl::proxy_req_t
              *     storage ������Ϊ idl::storage_req_t
             **/
             class _IdlReq,
             /**
              * idl ��Ӧ��:
              *     proxy ������Ϊ idl::proxy_res_t
              *     storage ������Ϊ idl::storage_res_t
             **/
             class _IdlRes,
             /**
              * idl ʹ�õ�mempool����, Ĭ��Ϊsyspool
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
         * @brief server�ص�����ԭ��
         * @param [in]     nshead     : ����nshead
         * @param [in]     req        : ����idl�ṹ��
         * @param [out]    res        : ��Ӧidl�ṹ��
         * @param [in/out] user_data  : ����nshead + idl_req_t
         * @param [in/out] pool       : idl mempool
         * @return  int �Ƿ����
         * @retval  _NAMESPACE_::SUCC:ok _NAMESPACE_::FAIL:����,�ر�����
        **/
        typedef int (*callback_func_t)(const nshead_t &nshead,
                                       const idl_req_t &req,
                                       idl_res_t &res,
                                       void *user_data,
                                       idl_pool_t& pool);

        /**
         * @brief �쳣����ص�, ub�ص�������idl���л���mcpack���쳣ʱ����
         *        ��Ҫ����: ��¼log, ���sf_ams���,
         *                  �ڼ��������л�������ɾ��β����¼
         *
         * @param [in]     e       : �׳���bsl::Exception
         * @param [in/out] res     : idl ��Ӧ��
         * @param [in]     on_save : �Ƿ�idl.save�����׳����쳣, false:no,true:yes
         *
         * @return int �Ƿ������쳣���� 0:������ 1:����
         * @date 2010/06/12 10:39:09
        **/
        typedef int (*exception_handle_t)(const bsl::Exception &e,
                                          idl_res_t *res,
                                          bool on_save);

        /**
         * @brief �̹߳���������ʼ������
         *
         * @param [in]     ithrd    : �̺߳� (0 - thrad_num)
         * @param [in/out] user_buf : �û��̹߳�������
         *
         * @return  int �Ƿ�ɹ� _NAMESPACE_::SUCC:�ɹ� _NAMESPACE_::FAIL:ʧ��
         * @date 2010/06/12 11:09:34
        **/
        typedef int (*shared_init_t)(uint32_t ithrd, void* shared_buf);

        //! �̹߳��������������, �ͷ��ڲ�������, ���ͷ�ָ�뱾��
        typedef shared_init_t shared_recycle_t;

    public:

        /**
         * @brief ���캯��
        **/
        Server():
            _inited(false), _ub_server(NULL), _callback(NULL),
            _shared_recycle(NULL), _exception_handle(NULL),
            _arr_shared(NULL)
        {// empty
        }

        /**
         * @brief de-ctor, �ͷ�Ctor�з������Դ
        **/
        virtual ~Server();

        /**
         * @brief ��ʼ����������
         *
         * @param [in] svr_conf        : server���õ�Ԫ,����ip/�˿�/�߳���/��дbuf��С����Ϣ
         * @param [in] shared_size     : �̹߳���ռ��С
         * @param [in] shared_init     : �̹߳���ռ��ʼ������
         * @param [in] exception_handle: �쳣�ص�
         *
         * @return int always return _NAMESPACE_::SUCC, ʧ��ֱ���˳�����
        **/
        int init(const comcfg::ConfigUnit& svr_conf,
                 const uint32_t shared_size,
                 shared_init_t shared_init,
                 shared_recycle_t shared_recycle,
                 exception_handle_t exception_handle);

        /**
         * @brief ����server
         *
         * @param [in] callback   : server�ص�����
         * @return  int �Ƿ�ɹ� _NAMESPACE_::SUCC:�ɹ� _NAMESPACE_::FAIL:ʧ��
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
         * ��ub_server ʹ�õ��̹߳�������
         * ����Server��ʼ��ʱ���
        **/
        struct _shared_wrapper_t
        {
            void *user_data; //!< �û�ʹ�õ��̹߳�������,�û����н���

            _Self *svr_handle; //!< �̶߳�Ӧ��Serverʵ��
            idl_pool_t *pool; //!< idlʹ�õ�mempool

            //rcpool_t *rcpool; //!< mcpackʹ�õ�resoucePool

            char *mcpack_tmp; //!< mcpack ʹ�õ� tmp buffer
            uint32_t mcpack_tmp_size; //!< mcpack_tmp ��С(bytes);

        };

        /**
         * @brief ��ʼ��_ub_svr_conf, ub_server������
         * @param [in] cgf_unit   : �����Ӧ���õ�Ԫ
         * @return  int �Ƿ�ɹ� _NAMESPACE_::SUCC:�ɹ� _NAMESPACE_::FAIL:ʧ��
        **/
        int _init_svr_conf(const comcfg::ConfigUnit& cgf_unit);

        /**
         * @brief ��ʼ��_arr_shared�û��̹߳���������������
         * @param [in] shared_size    : �û�buf��С
         * @param [in] shared_init    : ��ʼ���û�buf����
         * @param [in] shared_recycle : �����û�buf����
         * @return  int �Ƿ�ɹ� _NAMESPACE_::SUCC:�ɹ� _NAMESPACE_::FAIL:ʧ��
        **/
        int _init_shared_buf(const comcfg::ConfigUnit &svr_conf,
                             const uint32_t shared_size,
                             shared_init_t shared_init,
                             shared_recycle_t shared_recycle);

        /**
         * @brief ub �ص�����
         *
         * @return  int _NAMESPACE_::SUCC:�ɹ� _NAMESPACE_::FAIL:�����ر�����
        **/
        static int _callback_wrapper();

        /**
         * @brief ��mcpack ��������load idl������, ��������mcpack����
         *
         * @param [in]  mcpack_buf      : mcpack buffer
         * @param [in]  mcpack_buf_size : size of mcpack buffer
         * @param [in]  rcpool          : mcpackʹ�õ�resource pool
         * @param [out] req             : idl ������
         * @return  int �Ƿ�ɹ� _NAMESPACE_::SUCC:�ɹ� _NAMESPACE_::FAIL:ʧ��
        **/
        static int _load_idl_req(char *mcpack_buf,
                                 uint32_t mcpack_buf_size,
                                 _shared_wrapper_t *shared_data,
                                 idl_req_t &req);

        /**
         * @brief ��idl ��Ӧ�����л���mcpack��
         *        �ڲ������쳣�ص�, ����buffer�������
         * @param [in]  res        : idl ��Ӧ��
         * @param [in]  shared_data: �̹߳���ռ�
         * @param [out] mcpack_buf : mcpack buffer
         * @param [out] nshead_res : ��Ӧnshead
         * @return  int �Ƿ�ɹ� _NAMESPACE_::SUCC:�ɹ� _NAMESPACE_::FAIL:ʧ��
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

        bool _inited;     //!< �Ƿ��ѳ�ʼ��
        ub_server_t *_ub_server;      //!< ub_server ʵ��
        ub_svr_t _ub_svr_conf;    //!< ub server ������
        callback_func_t _callback;    //!< �û��ص�
        shared_recycle_t _shared_recycle;     //!< �û�������������
        exception_handle_t _exception_handle;     //!< �쳣����ص�


        //!< �̹߳�������,���캯���з��䡢�������ͷ�
        void **_arr_shared;
    };

}

#include "ubsvr_impl.h"

#endif
