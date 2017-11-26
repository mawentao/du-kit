/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: g_pack_t.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   g_pack_t.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  全局结构体实现
 **/

#include "g_pack_t.h"

/** 初始化data_block_t */
int data_block_t::init(uint32_t buf_size, uint32_t data_block_size)
{/*{{{*/
    //1. 初始化buf
    buf = new char *[data_block_size];
    if(!buf){
        UB_LOG_WARNING("init data_block_t fail [size:%u].", data_block_size);
        return -1;
    }
    for(uint32_t i=0; i<data_block_size; ++i){
        buf[i] = new char[buf_size];
        if(!buf[i]){
            UB_LOG_WARNING("create data_block_t::buf fail [buf_size:%u].", buf_size);
            return -1;
        }
    }
    //2. 初始化其他
    size = 0;
    state = 0;

    return 0;
}/*}}}*/


//////////////////////////////////////////////////////////////////////////////////


/** 获取可执行程序所在的目录  */
void g_pack_t::get_exe_path()
{/*{{{*/
    int ret = Util::get_exe_path(exe_path, FILE_NAME_LEN);
    if(0 != ret){
        fprintf(stderr, "get_exe_path fail.\n");
        exit(-1);
    }
}/*}}}*/


/** 读取配置，初始化 */
void g_pack_t::init(comcfg::ConfigUnit &conf)
{/*{{{*/
    //1. 读取配置
    line_buf_size = conf["line_buf_size"].to_uint32();
    data_block_size = conf["data_block_size"].to_uint32();
    data_block_arr_size = conf["data_block_arr_size"].to_uint32();
    process_thread_num = conf["process_thread_num"].to_uint32();
    max_col_num = conf["max_col_num"].to_uint32();
    max_col_len = conf["max_col_len"].to_uint32();
    keep_order = conf["keep_order"].to_uint32();

    //2. 创建数据块队列并初始化
    block_arr = new data_block_t[data_block_arr_size];
    if(!block_arr){
        UB_LOG_FATAL("create block_arr fail [size:%u].", data_block_arr_size);
        exit(-1);
    }
    for(uint32_t i=0; i<data_block_arr_size; ++i){
        if(0 != block_arr[i].init(line_buf_size, data_block_size)){
            UB_LOG_FATAL("init block_arr[%u] fail.", i);
            exit(-1);
        }
    }

    //3. 初始化其他
    last_read_index = 0 ;
    last_write_index = 0;
    last_process_index = 0;
    read_finish = false;
}/*}}}*/


/** 调试用 */
void g_pack_t::output(FILE *fp)
{/*{{{*/
    fprintf(fp, "g_pack: {\n");
    fprintf(fp, "    %-20s: %s\n",  "exe_path", exe_path);
    fprintf(fp, "    %-20s: %s\n",  "input_file", input_file);
    fprintf(fp, "    %-20s: %s\n",  "output_file", output_file);
    fprintf(fp, "    %-20s: %lu\n\n", "pro_line_num", pro_line_num);
    fprintf(fp, "    %-20s: %lu\n", "line_buf_size", line_buf_size);
    fprintf(fp, "    %-20s: %lu\n", "data_block_size", data_block_size);
    fprintf(fp, "    %-20s: %lu\n", "data_block_arr_size", data_block_arr_size);
    fprintf(fp, "    %-20s: %lu\n", "process_thread_num", process_thread_num);
    fprintf(fp, "    %-20s: %lu\n", "max_col_num", max_col_num);
    fprintf(fp, "    %-20s: %lu\n", "max_col_len", max_col_len);
    fprintf(fp, "    %-20s: %lu\n", "keep_order", keep_order);
    fprintf(fp, "}\n");
}/*}}}*/



/** 获取一个指定状态的数据块 */
data_block_t * get_block(g_pack_t &g, int block_state, uint64_t index)
{/*{{{*/
    data_block_t *res = NULL;

    uint32_t scan_num = 0;

    while(!res){
        //进入临界区
        pthread_mutex_lock(&g.block_lock);

        //获取一个满足state条件的block
        //找一个可以读的block
        if(0 == block_state){
            uint32_t &i = g.last_read_index;
            for(; i<g.data_block_arr_size; ++i){
                if(g.block_arr[i].state == block_state){
                    res = &g.block_arr[i];
                    res->state = 1;
                    res->size = 0;
                    break;
                }
            }
            if(i == g.data_block_arr_size){i=0;}
        }else if(2 == block_state){
        //找一个可以处理的block
            uint32_t &i = g.last_process_index;
            for(; i<g.data_block_arr_size; ++i){
                if(g.block_arr[i].state == block_state){
                    res = &g.block_arr[i];
                    res->state = 3;
                    break;
                }
            }
            if(i == g.data_block_arr_size){i=0;}
            if(g.read_finish){
                ++scan_num;
            }
        }else if(4 == block_state){
        //找一个可以写的block
            uint32_t &i = g.last_write_index;
            bool process_finish = true;
            for(; i<g.data_block_arr_size; ++i){
                if(g.block_arr[i].state == block_state){
                    process_finish = false;
                    if(g.keep_order && g.block_arr[i].index != index){
                        //如果需要保持输出顺序，则要获取给定index的数据块
                        continue;
                    }
                    res = &g.block_arr[i];
                    res->state = 5;
                    break;
                }else{
                    if(g.block_arr[i].state>0 && g.block_arr[i].state<4){
                        process_finish = false;
                    }
                }
            }
            if(i == g.data_block_arr_size){i=0;}
            if(g.read_finish && process_finish){
                ++scan_num;
            }
        }

        //退出临界区
        pthread_mutex_unlock(&g.block_lock);

        //上个过程处理完毕，退出
        if(scan_num >= 2){break;}
    }

    return res;
}/*}}}*/




// vim:fdm=marker:nu:ts=4:sw=4:expandtab
