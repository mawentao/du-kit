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
    //1. init line_buf
    line_buf_size = conf["line_buf_size"].to_uint32();
    line_buf = new char[line_buf_size];
    if(!line_buf){
        fprintf(stderr, "init line_buf fail [size:%u].\n", line_buf_size);
        exit(-1);
    }

    //2. init cols
    max_col_num = conf["max_col_num"].to_uint32();
    max_col_len = conf["max_col_len"].to_uint32();
    col_num = 0;
    cols = new char*[max_col_num];
    if(!cols){
        fprintf(stderr, "init clos fail [size:%u].\n", max_col_num);
        exit(-1);
    }
    for(uint32_t i=0; i<max_col_num; ++i){
        cols[i] = new char[max_col_len];
        if(!cols[i]){
            fprintf(stderr, "init cols[%u] fail [size:%u].\n", i, max_col_len);
            exit(-1);
        }
    }
}/*}}}*/

/** 按指定字符分割行 */
void g_pack_t::split_line(char c)
{/*{{{*/
    col_num = 0;
    const char *ps = line_buf;
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
