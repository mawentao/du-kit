/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: data_proc.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   data_proc.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  ���ݴ���ģ���ʵ��
 **/

#include "data_proc.h"

/** ��ʼ�� */
int DataProc::init(comcfg::ConfigUnit &conf)
{/*{{{*/
    //1. ��ʼ���߳�
    m_thread_num = conf["process_thread_num"].to_uint32();
    m_threads = new pthread_t[m_thread_num];
    if(!m_threads){
        UB_LOG_FATAL("create DataProc threads fail [thread_num:%u].", m_thread_num);
        return -1;
    }

    //2. ��ʼ��ubclient����
    if(0 != m_pack.ubclient_conf.init(conf["ubserver"])){
        UB_LOG_FATAL("init ubclient fail.");
        return -1;
    }

    return 0;
}/*}}}*/


/** �̺߳��� */
void * DataProc::m_thread_func(void *args)
{/*{{{*/
    uint64_t tid = pthread_self();

    //1. ���߳���־
    ub_log_initthread("p_data_process");
    UB_LOG_TRACE("���ݴ����߳�������[%lu]\n", tid);
    data_proc_pack_t &pack = *((data_proc_pack_t *)args);
    g_pack_t &g = *pack.g_pack;
    data_block_t *cur_block = get_block(g, 2);  //��ǰ���ݿ�
//    printf("tid: %lu; ubc: %lu\n", tid, (uint64_t)pack.ubc);
    uint32_t i=0;

    //2. ����ǰ�ĳ�ʼ��
    t_pack_t t_pack(g.max_col_num, g.max_col_len);
    NqcClient ubc;
    if(0 != ubc.init(pack.ubclient_conf)){
        UB_LOG_FATAL("create UBClient fail.");
        suicide();
    }

    //3. ȡ���ݿ鴦��
    while(cur_block){
//        printf("�������ݿ飺%lu\n", cur_block->index);
        for(i=0; i<cur_block->size; ++i){
            //3-1. �ָ���
            t_pack.split_line(cur_block->buf[i], '\t');
            //strcat(cur_block->buf[i], " done");
            //3-2. ����ÿһ���ֶ�
            snprintf(cur_block->buf[i], g.line_buf_size, "%s", t_pack.cols[0]);
//            ubc.do_process();   //����UBServer����
/*
            for(uint32_t i=0; i<t_pack.col_num; ++i){
                strcat(cur_block->buf[i], t_pack.cols[i]);
            }
*/
        }
        cur_block->state = 4;
        //��ȡ��һ�����ݰ�
        cur_block = get_block(g, 2);
    }

    //4. �ر��߳���־
    UB_LOG_TRACE("�����߳̽���. [tid:%lu]\n", tid);
    ub_log_closethread();

    return NULL;
}/*}}}*/


//////////////////////////////////////////////////////////////////////////////////


t_pack_t::t_pack_t(uint32_t max_colnum, uint32_t max_collen):
cols(NULL), col_num(0), max_col_num(max_colnum), max_col_len(max_collen)
{/*{{{*/
    cols = new char*[max_col_num];
    if(!cols){
        UB_LOG_FATAL("create cols fail [size:%u].", max_col_num);
        suicide();
    }
    for(uint32_t i=0; i<max_col_num; ++i){
        cols[i] = new char[max_col_len];
        if(!cols[i]){
            UB_LOG_FATAL("create cols[%u] fail [size:%u].\n", i, max_col_len);
            suicide();
        }
    }
}/*}}}*/

t_pack_t::~t_pack_t()
{/*{{{*/
    for(uint32_t i=0; i<max_col_num; ++i){
        delete [] cols[i];
    }
    delete [] cols;
}/*}}}*/

/** ��ָ���ַ��ָ��� */
void t_pack_t::split_line(const char *line, char c)
{/*{{{*/
    col_num = 0;
    const char *ps = line;
    char *pt = cols[col_num];
    *pt = '\0';
    while(*ps){
        if(*ps == c){
            *pt = '\0';
            ++col_num;
            pt = cols[col_num];
            ++ps;
        }else{
            *pt++ = *ps++;
        }
    }
    *pt='\0';
    ++col_num;
}/*}}}*/




// vim:fdm=marker:nu:ts=4:sw=4:expandtab
