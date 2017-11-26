/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: auto_res.h, v1.0.0 __DATETIME__, Exp $
 *
 **************************************************************************/

/**
 * @file   auto_res.h
 * @date   __DATETIME__
 * @brief  ��Դ�Զ��ͷ�ģ����
 **/

#ifndef AUTO_RES_H
#define AUTO_RES_H

#include "ul_net.h"
#include "odict.h"
#include "ub_server.h"

namespace _NAMESPACE_
{
    /**
     *      \brief ��Դ�Զ��ͷ�ģ����.
     *
     *      �����ʵ������������ڽ�������Զ��ͷ���Դ
     *      ʹ�÷���:
     *      auto_res_t<FILE *, int> fp(NULL, fclose);
     *      fp() = fopen(...)
     *
     *      \param[in] ValueType
     *           ��Դԭ���ͣ��� FILE *, sodict_build_t *, ��
     *      \param[in] FinalizeFuncRetType
     *           �ͷŸ���Դ�ĺ����ķ���ֵ, �� finalize_func_t �ķ���ֵ, ���� void free(void *ptr) ��Ӧ�����;��� void
     *      \param[in] default_value
     *           ��Դ��Ĭ��ֵ, �� fd: -1, FILE *: NULL
     *           �������ڽ���ʱ����Դֵ����Ĭ��ֵ������ͷ�
     *      \param[in] finalize_func
     *           ��Դ�ͷŵĺ���
     */
    template <typename T_val, typename T_finalize_func_ret>
    class auto_res_t
    {/*{{{*/
    public:
        typedef T_val value_t;
        typedef T_finalize_func_ret (*finalize_func_t)(value_t);
    protected:
        value_t m_var;
        const value_t m_default_value;
        const finalize_func_t m_finalize_func;
    public:
        /**
         *      ���캯��
         *
         *      @param[in] default_value        ��Դ��Ĭ��ֵ, ����������ִ��ʱ, ����Դ��ֵ������Ĭ��ֵ, �������Ӧ�Ļ��պ���
         *      @param[in] finalize_func        ��Դ�Ļ��պ���
         */
        auto_res_t(value_t default_value, finalize_func_t finalize_func)
            :m_var(default_value), m_default_value(default_value), m_finalize_func(finalize_func)
        {}
        virtual ~auto_res_t()
        {
            if (m_var != m_default_value)
            {
                if (m_finalize_func)
                {
                    m_finalize_func(m_var);
                }
                m_var = m_default_value;
            }
        }

        /**
         *      operator() ����ȡ��ʵ�ʵ���Դ�������в���
         */
        value_t &operator() ()
        {
            return m_var;
        }
    private:
        auto_res_t(const auto_res_t<T_val, T_finalize_func_ret> &); // disabled
        auto_res_t<T_val, T_finalize_func_ret> &operator =(auto_res_t<T_val, T_finalize_func_ret>);
    };/*}}}*/

    /**
     *      @brief ���� auto_res_t ������ FILE * ��Դ
     */
    class auto_fp_t: public auto_res_t<FILE *, int>
    {/*{{{*/
    public:
        auto_fp_t()
            :auto_res_t<FILE *, int>(NULL, fclose)
        {}
    };/*}}}*/

    /**
     *      @brief ���� auto_res_t ������ int fd ��Դ
     */
    class auto_fd_t: public auto_res_t<int, int>
    {/*{{{*/
    public:
        auto_fd_t()
            :auto_res_t<int, int>(-1, ul_sclose)
        {}
    };/*}}}*/

    /**
     *      @brief ���� auto_res_t ������ָ����Դ (�� free �ͷ�)
     *
     *      @param[in] ָ��ָ�����������
     */
    template <typename T>
    class auto_ptr_t: public auto_res_t<T *, void>
    {/*{{{*/
    protected:
        static void _free(T *ptr)
        {
            free((void *)ptr);
        }
    public:
        auto_ptr_t()
            :auto_res_t<T *, void>(NULL, _free)
        {}
    };/*}}}*/

    /**
     *      @brief ���� auto_res_t ������ sodict_build_t * ��Դ
     */
    class auto_odb_t: public auto_res_t<sodict_build_t *, int>
    {/*{{{*/
    public:
        auto_odb_t()
            :auto_res_t<sodict_build_t *, int>(NULL, odb_destroy)
        {}
    };/*}}}*/

    /**
     *      @brief ���� auto_res_t ������ ub_server_t * ��Դ
     */
    class auto_ub_server_t: public auto_res_t<ub_server_t *, int>
    {/*{{{*/
    public:
        auto_ub_server_t()
            :auto_res_t<ub_server_t *, int>(NULL, ub_server_destroy)
        {}
    };/*}}}*/



    /**
     *      @brief ��� memory pool �����ڴ�ָ��
     */
    template <typename T, typename T_pool>
    class auto_pool_ptr_t
    {/*{{{*/
    protected:
        T *_ptr;
        T_pool &_pool;
        size_t _size;
    public:
        auto_pool_ptr_t(T_pool &pool)
            :_ptr(NULL), _pool(pool), _size(0)
        {
        }

        T *malloc(uint32_t num)
        {
            _size = num * sizeof(T);
            _ptr = (T *)_pool.malloc(_size);
            // ��Ҫ���û��ж��Ƿ�ʧ�� 
            return _ptr;
        }

        ~auto_pool_ptr_t()
        {
            if (_ptr)
            {
                _pool.free(_ptr, _size);
                _ptr = NULL;
            }
        }

        const T *operator () () const
        {
            return _ptr;
        }

        T *operator () ()
        {
            return _ptr;
        }
    };/*}}}*/



}


#endif
