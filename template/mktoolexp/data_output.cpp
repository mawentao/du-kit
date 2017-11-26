/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: data_output.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   data_output.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  �������ģ���ʵ��
 **/

#include "data_output.h"


/** ��ʼ�� */
int DataOutput::init(comcfg::ConfigUnit &conf)
{/*{{{*/
    return 0;
}/*}}}*/


/** �̺߳��� */
void * DataOutput::m_thread_func(void *args)
{
    uint64_t tid = pthread_self();

    //1. ���߳���־
    ub_log_initthread("p_data_write");
    UB_LOG_TRACE("д�߳�������[%lu]\n", tid);
    g_pack_t &g = *((g_pack_t *)args);

    //2. ������ļ�
    FILE *fo = fopen(g.output_file, "w");
    if(!fo){
        UB_LOG_FATAL("open output file fail [file:%s].", g.output_file);
        suicide();
    }

    uint64_t index = 1;
    data_block_t *cur_block = get_block(g, 4, index);  //��ǰ���ݿ�
    uint32_t i=0;

    while(cur_block){
//        printf("������ݿ�: %lu\n", cur_block->index);
        for(i=0; i<cur_block->size; ++i){
            fprintf(fo, "%s\n", cur_block->buf[i]);
        }
        cur_block->state = 0;
        ++index;
        cur_block = get_block(g, 4, index);
    }


    //3. �ر��߳���־
    fclose(fo);
    UB_LOG_TRACE("д�߳̽�����[%lu]\n", tid);
    ub_log_closethread();


    return NULL;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
