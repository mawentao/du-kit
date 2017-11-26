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


/** 分离全路径文件名的路径和文件名 */
int split_full_path_file(const char *fullpath, char *path, int path_size, char * file, int file_size)
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
    if(need_path_size > path_size || need_file_size > file_size){
        return -1;
    }
    memset(path, 0, path_size);
    memset(file, 0, file_size);
    strncpy(path, fullpath, split_place);
    strncpy(file, fullpath+split_place, need_file_size-1);
    return 0;
}/*}}}*/


/** 获取可执行程序的路径 */
int get_exe_path(char *buf, int size)
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
