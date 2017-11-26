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
 * @brief  数据输入模块的实现
 **/

#include "data_input.h"


/** 初始化 */
int DataInput::init(comcfg::ConfigUnit &conf)
{/*{{{*/
    return 0;
}/*}}}*/


/** 线程函数 */
void * DataInput::m_thread_func(void *args)
{/*{{{*/
    uint64_t tid = pthread_self();

    //1. 打开线程日志
    ub_log_initthread("p_data_read");
    UB_LOG_TRACE("读线程启动：[%lu]\n", tid);
    g_pack_t &g = *((g_pack_t *)args);

    //2. 打开输入文件
    FILE *fi = fopen(g.input_file, "r");
    if(!fi){
        UB_LOG_FATAL("open input file fail [file:%s].", g.input_file);
        suicide();
    }

    //3. 处理文件
    uint32_t cur = 0;               //当前行号
    uint32_t sum = g.pro_line_num;  //总行数
    uint32_t last = 0;              //上次打印进度的行号
    data_block_t *cur_block = NULL; //当前数据块
    g.read_finish = false;   //未输入完毕
    char *line_buf = new char[g.line_buf_size];
    if(!line_buf){
        UB_LOG_FATAL("read_thread: create line_buf fail [size:%u].", g.line_buf_size);
        suicide();
    }
    uint64_t index = 0;     // 数据块序号

    while(fgets(line_buf, g.line_buf_size, fi))
    {
        //3-1. 去除行尾空白字符
        Util::str_rtrim(line_buf);

        //3-2. 如果当前数据块为空，或数据已经填满，则取一个新的
        if(!cur_block){
            cur_block = get_block(g, 0);
            if(!cur_block){
                UB_LOG_FATAL("get block fail.");
                suicide();
            }
            ++index;
            cur_block->index = index;
        }

        //3-3. 放入数据块
        snprintf(cur_block->buf[cur_block->size], g.line_buf_size, "%s", line_buf);
//        printf("放入数据块: %s\n", line_buf);
        ++cur_block->size;
        if(cur_block->size >= g.data_block_size){
            cur_block->state = 2;
            cur_block = NULL;
        }

        //3-4. 打印进度
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

    //2. 关闭文件&线程日志
    fclose(fi);
    UB_LOG_TRACE("读线程结束. [tid:%lu]\n", tid);
    ub_log_closethread();
    return NULL;

}/*}}}*/


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
