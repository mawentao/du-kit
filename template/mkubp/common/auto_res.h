/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: auto_res.h, v1.0.0 __DATETIME__, Exp $
 *
 **************************************************************************/

/**
 * @file   auto_res.h
 * @date   __DATETIME__
 * @brief  资源自动释放模板类
 **/

#ifndef AUTO_RES_H
#define AUTO_RES_H

#include "ul_net.h"
#include "odict.h"
#include "ub_server.h"

namespace _NAMESPACE_
{
    /**
     *      \brief 资源自动释放模板类.
     *
     *      该类的实例当类的生存期结束后可自动释放资源
     *      使用方法:
     *      auto_res_t<FILE *, int> fp(NULL, fclose);
     *      fp() = fopen(...)
     *
     *      \param[in] ValueType
     *           资源原类型，如 FILE *, sodict_build_t *, 等
     *      \param[in] FinalizeFuncRetType
     *           释放该资源的函数的返回值, 即 finalize_func_t 的返回值, 例如 void free(void *ptr) 对应的类型就是 void
     *      \param[in] default_value
     *           资源的默认值, 如 fd: -1, FILE *: NULL
     *           当生存期结束时若资源值等于默认值则进行释放
     *      \param[in] finalize_func
     *           资源释放的函数
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
         *      构造函数
         *
         *      @param[in] default_value        资源的默认值, 当析构函数执行时, 若资源的值不等于默认值, 则调用相应的回收函数
         *      @param[in] finalize_func        资源的回收函数
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
         *      operator() 用来取出实际的资源变量进行操作
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
     *      @brief 基于 auto_res_t 派生的 FILE * 资源
     */
    class auto_fp_t: public auto_res_t<FILE *, int>
    {/*{{{*/
    public:
        auto_fp_t()
            :auto_res_t<FILE *, int>(NULL, fclose)
        {}
    };/*}}}*/

    /**
     *      @brief 基于 auto_res_t 派生的 int fd 资源
     */
    class auto_fd_t: public auto_res_t<int, int>
    {/*{{{*/
    public:
        auto_fd_t()
            :auto_res_t<int, int>(-1, ul_sclose)
        {}
    };/*}}}*/

    /**
     *      @brief 基于 auto_res_t 派生的指针资源 (用 free 释放)
     *
     *      @param[in] 指针指向的数据类型
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
     *      @brief 基于 auto_res_t 派生的 sodict_build_t * 资源
     */
    class auto_odb_t: public auto_res_t<sodict_build_t *, int>
    {/*{{{*/
    public:
        auto_odb_t()
            :auto_res_t<sodict_build_t *, int>(NULL, odb_destroy)
        {}
    };/*}}}*/

    /**
     *      @brief 基于 auto_res_t 派生的 ub_server_t * 资源
     */
    class auto_ub_server_t: public auto_res_t<ub_server_t *, int>
    {/*{{{*/
    public:
        auto_ub_server_t()
            :auto_res_t<ub_server_t *, int>(NULL, ub_server_destroy)
        {}
    };/*}}}*/



    /**
     *      @brief 配合 memory pool 管理内存指针
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
            // 需要由用户判断是否失败 
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
