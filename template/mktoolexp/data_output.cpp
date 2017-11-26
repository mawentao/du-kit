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
 * @brief  数据输出模块的实现
 **/

#include "data_output.h"


/** 初始化 */
int DataOutput::init(comcfg::ConfigUnit &conf)
{/*{{{*/
    return 0;
}/*}}}*/


/** 线程函数 */
void * DataOutput::m_thread_func(void *args)
{
    uint64_t tid = pthread_self();

    //1. 打开线程日志
    ub_log_initthread("p_data_write");
    UB_LOG_TRACE("写线程启动：[%lu]\n", tid);
    g_pack_t &g = *((g_pack_t *)args);

    //2. 打开输出文件
    FILE *fo = fopen(g.output_file, "w");
    if(!fo){
        UB_LOG_FATAL("open output file fail [file:%s].", g.output_file);
        suicide();
    }

    uint64_t index = 1;
    data_block_t *cur_block = get_block(g, 4, index);  //当前数据块
    uint32_t i=0;

    while(cur_block){
//        printf("输出数据块: %lu\n", cur_block->index);
        for(i=0; i<cur_block->size; ++i){
            fprintf(fo, "%s\n", cur_block->buf[i]);
        }
        cur_block->state = 0;
        ++index;
        cur_block = get_block(g, 4, index);
    }


    //3. 关闭线程日志
    fclose(fo);
    UB_LOG_TRACE("写线程结束：[%lu]\n", tid);
    ub_log_closethread();


    return NULL;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
