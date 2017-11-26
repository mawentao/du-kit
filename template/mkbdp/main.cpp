/***************************************************************************
 *
 * Copyright (c) _YEAR_ _AUTHOR_, Inc. All Rights Reserved
 * $Id: main.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   main.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  主程序入口
 **/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "_FILENAME_.h"
using namespace std;


/** 解析命令行参数，读取配置并初始化 */
int parse_args(int argc, char *argv[])
{
    //1. load conf file
    char conf_path[] = "./conf";
    char conf_file[] = "_PROJECTNAME_.conf";
    char conf_file_range[] = "_PROJECTNAME_.conf.range";
    comcfg::Configure cfg;
    int ret = cfg.load(conf_path, conf_file, conf_file_range);
    if(ret != 0){
        return -1;
    }

    //2. 读取日志配置，并打开日志
    char log_path[1024];
    char log_file[1024];
    uint32_t log_level;
    uint32_t log_size;
    strncpy(log_path, cfg["log_path"].to_cstr(), 1024);
    strncpy(log_file, cfg["log_file"].to_cstr(), 1024);
    log_level = cfg["log_level"].to_uint32();
    log_size = cfg["log_size"].to_uint32();
    ret = ub_log_init(log_path, log_file, log_size, log_level);
    if(ret != 0){
        fprintf(stderr, "open log fail.\n");
        exit(-1);
    }

    return 0;
}


/** 主程序 */
int main(int argc, char *argv[])
{

    //1. 解析命令行参数，读取配置并初始化
    int ret = parse_args(argc, argv);
    if(0 != ret){
        printf("parse_args fail.");
    }

    cout<<"!!!THE END!!!"<<endl;
    return 0;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
