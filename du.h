/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: du.h, v1.0.0, 2015-12-30 10:40:13, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   du.h
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:40:13
 * @brief  du.h
 **/

#ifndef DU_H
#define DU_H

#include "Configure.h"

#define VERSION "v3.6 (mawentao) 2017-12-21"
#define MAX_FILENAME_LEN 1024


/* 一些生成文件时需要被替换的字段 */
struct du_t{
    char exe_path[MAX_FILENAME_LEN];  //!< 执行程序所在目录
    char cur_path[MAX_FILENAME_LEN];  //!< 当前路径
    char temp_path[MAX_FILENAME_LEN]; //!< 模板文件路径

    char _YEAR_[16];       //!< 替换字段_YEAR_
    char _DATETIME_[32];   //!< 替换字段_DATETIME_
    char _AUTHOR_[MAX_FILENAME_LEN];         //!< 替换字段 _AUTHOR_
    char _FILENAME_[MAX_FILENAME_LEN];       //!< 替换字段 _FILENAME_不包括.后缀
    char _HEAD_FILENAME_[MAX_FILENAME_LEN];  //!< 替换字段 _HEAD_FILENAME_(主要用于头文件中的#ifndef _HEAD_FILENAME_)
    char _CLASSNAME_[MAX_FILENAME_LEN];      //!< 替换字段 _CLASSNAME_(生成类名)
    char _WORK_ROOT_[MAX_FILENAME_LEN];      //!< 替换字段 _WORK_ROOT_(百度SVNROOT的相对路径)

    /** for debug */
    void output(FILE *fp=stdout){
        fprintf(fp, "exe_path: %s\n", exe_path);
        fprintf(fp, "cur_path: %s\n", cur_path);
        fprintf(fp, "temp_path: %s\n", temp_path);
        fprintf(fp, "_YEAR_: %s\n", _YEAR_);
        fprintf(fp, "_DATETIME_: %s\n", _DATETIME_);
        fprintf(fp, "_AUTHOR_: %s\n", _AUTHOR_);
        fprintf(fp, "_FILENAME_: %s\n", _FILENAME_);
        fprintf(fp, "_HEAD_FILENAME_: %s\n", _HEAD_FILENAME_);
        fprintf(fp, "_WORK_ROOT_: %s\n", _WORK_ROOT_);
    }
};

void print_usage(const char *p);

/* 解析命令行参数，读取配置并初始化 */
int parse_args(int argc, char *argv[], du_t &du);

/* 初始化du_t */
int init_du(du_t &du, const char *conf_path="./conf", const char *conf_file="main.ini");

/** 删除.后缀 */
char* trip_suffix(char *str);

/** 获取上层目录 */
void get_parent(char *path);

/** 判断是否是文件 */
bool is_file(const char *fname);

/** 判断是否是目录 */
bool is_dir(const char *fname);

/** 执行sed命令 */
void exe_sed(const char *src, const char *dest, const char *sed);

/** 把str中的所有字母转成小写字母 */
char* lower(char *str);

/** 把str中的所有字母转成大写字母 */
char* upper(char *str);

/** 根据文件名生成类名 */
void generate_class_name(const char *filename, char *classname);

/** 生成文件 */
void gen_file(const char *tempfile, const char *desfile, du_t &du, bool relace=false);

/** Copy文件 */
void copy_file(const char *tempfile, const char *desfile, bool relace=false);

/** 判断是否是work_root */
bool is_work_root(const char *path);

/** 检查是否在baidu svn_root下 */
bool check_work_root(char *path, char *WORK_ROOT);

inline void print_exe(const char *msg)
{
    printf(">>> %s\n", msg);
}

inline void exe_cmd(const char *cmd)
{
    system(cmd);
    print_exe(cmd);
}

#endif
