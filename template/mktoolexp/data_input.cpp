/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: data_input.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   data_input.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  ��������ģ���ʵ��
 **/

#include "data_input.h"


/** ��ʼ�� */
int DataInput::init(comcfg::ConfigUnit &conf)
{/*{{{*/
    return 0;
}/*}}}*/


/** �̺߳��� */
void * DataInput::m_thread_func(void *args)
{/*{{{*/
    uint64_t tid = pthread_self();

    //1. ���߳���־
    ub_log_initthread("p_data_read");
    UB_LOG_TRACE("���߳�������[%lu]\n", tid);
    g_pack_t &g = *((g_pack_t *)args);

    //2. �������ļ�
    FILE *fi = fopen(g.input_file, "r");
    if(!fi){
        UB_LOG_FATAL("open input file fail [file:%s].", g.input_file);
        suicide();
    }

    //3. �����ļ�
    uint32_t cur = 0;               //��ǰ�к�
    uint32_t sum = g.pro_line_num;  //������
    uint32_t last = 0;              //�ϴδ�ӡ���ȵ��к�
    data_block_t *cur_block = NULL; //��ǰ���ݿ�
    g.read_finish = false;   //δ�������
    char *line_buf = new char[g.line_buf_size];
    if(!line_buf){
        UB_LOG_FATAL("read_thread: create line_buf fail [size:%u].", g.line_buf_size);
        suicide();
    }
    uint64_t index = 0;     // ���ݿ����

    while(fgets(line_buf, g.line_buf_size, fi))
    {
        //3-1. ȥ����β�հ��ַ�
        Util::str_rtrim(line_buf);

        //3-2. �����ǰ���ݿ�Ϊ�գ��������Ѿ���������ȡһ���µ�
        if(!cur_block){
            cur_block = get_block(g, 0);
            if(!cur_block){
                UB_LOG_FATAL("get block fail.");
                suicide();
            }
            ++index;
            cur_block->index = index;
        }

        //3-3. �������ݿ�
        snprintf(cur_block->buf[cur_block->size], g.line_buf_size, "%s", line_buf);
//        printf("�������ݿ�: %s\n", line_buf);
        ++cur_block->size;
        if(cur_block->size >= g.data_block_size){
            cur_block->state = 2;
            cur_block = NULL;
        }

        //3-4. ��ӡ����
        ++cur;
        Util::print_progress(cur, sum, last);
        if(cur >= g.pro_line_num){
            break;
        }
    }

    if(cur_block){
        cur_block->state = 2;
    }
    g.read_finish = true;

    //2. �ر��ļ�&�߳���־
    fclose(fi);
    UB_LOG_TRACE("���߳̽���. [tid:%lu]\n", tid);
    ub_log_closethread();
    return NULL;

}/*}}}*/


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
