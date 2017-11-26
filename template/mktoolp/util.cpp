/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: util.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   util.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  util.cpp
 **/

#include "util.h"

/** 打印进度 */
void Util::print_progress(uint32_t cur, uint32_t sum, uint32_t &last)
{/*{{{*/
    //1. 计算当前进度
    uint32_t step = 0;
    if(sum < 100){
        step = cur * (100/sum);
    }else{
        step = (cur*100) / sum;
    }
    //2. 如果当前进度已更新，则显示
    if(step>=last && step <= 100){
        std::string ss = "";
        for(uint32_t i=0; i<step; ++i){
            ss += "#";
        }
        //FILE *fo = fopen("log", "a");
        //fprintf(fo, "progress: [%-100s] %d%%\n", ss.c_str(), step);
        //fclose(fo);
        fprintf(stdout, "progress: [%-100s] %d%%\r", ss.c_str(), step);
        fflush(stdout);
        ++last;
    }
}/*}}}*/


/** 去除尾部空白符(\n\r\t) */
void Util::str_rtrim(char *str)
{/*{{{*/
    uint32_t len = strlen(str);
    char *p = str + len - 1;
    while(p>=str){
        if(*p=='\n' || *p=='\r' || *p=='\t' || *p==' '){
            *p='\0';
            --p;
        }else{
            break;
        }
    }
}/*}}}*/


/** 统计文件行数 */
uint64_t Util::stat_line_num(const char *fname)
{/*{{{*/
    uint64_t line = 0;
    FILE *fp = fopen(fname, "r");
    if(fp){
        char buf[1024];
        while( fgets(buf, 1024, fp) ){ ++line; }
        fclose(fp);
    }
    return line;
}/*}}}*/


/** 分离全路径文件名的路径和文件名 */
int Util::split_full_path_file(const char *fullpath, char *path, uint32_t path_size, char * file, uint32_t file_size)
{/*{{{*/
    int size = 0;
    int split_place = 0;
    const char *p = fullpath;
    while(*p){
        ++size;
        if('/' == *p++){
            split_place = size;
        }
    }
    int need_path_size = split_place + 1;
    int need_file_size = size - split_place + 1;
    if(need_path_size > (int)path_size || need_file_size > (int)file_size){
        return 0;
    }
    memset(path, 0, path_size);
    memset(file, 0, file_size);
    strncpy(path, fullpath, split_place);
    strncpy(file, fullpath+split_place, need_file_size-1);
    return 1;
}/*}}}*/


/** 获取可执行程序的路径 */
int Util::get_exe_path(char *buf, uint32_t size)
{/*{{{*/
    char fullexe[1024];
    char tmp[1024];
    char *ptr;
    ptr = (char *)malloc((size_t)size);
    if(NULL != ptr){
        memset(ptr,0,size);
        snprintf(ptr, size, "/proc/%d/exe",getpid());
    } else {
        return -1;
    }
    readlink(ptr,fullexe,size);
    split_full_path_file(fullexe, buf, size, tmp, 1024);
    return 0;
}/*}}}*/

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
