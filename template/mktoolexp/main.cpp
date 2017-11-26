/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
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
#include "data_input.h"
#include "data_output.h"
#include "data_proc.h"
using namespace std;


//////////////////////////////////////////////

/* 全局内存 */
g_pack_t g_pack;

/* 数据输入模块 */
DataInput data_input;

/* 数据输出模块 */
DataOutput data_output;

/* 数据处理模块 */
DataProc data_proc;

//////////////////////////////////////////////


/** 打印帮助 */
void print_usage(const char *pname)
{/*{{{*/
    printf("toolp v1.0 (mawentao@baidu.com)\n");
    printf("\n[usage]:\n");
    printf("    %s input_file [-o output_file]\n", pname);
}/*}}}*/

/** 解析命令行参数，读取配置并初始化 */
void parse_args(int argc, char *argv[])
{/*{{{*/
    //1. 解析命令行参数
    if(argc < 2){
        print_usage(argv[0]);
        exit(-1);
    }
    uint32_t state = 1;
    for(int i=1; i<argc; ++i){
        if(strcmp(argv[i], "-o") == 0){
            state = 2;
            continue;
        }
        if(strcmp(argv[i], "-n") == 0){
            state = 3;
            continue;
        }
        switch(state){
        case 1:
            snprintf(g_pack.input_file, FILE_NAME_LEN, argv[i]);
            break;
        case 2:
            snprintf(g_pack.output_file, FILE_NAME_LEN, argv[i]);
            state = 1;
            break;
        case 3:
            g_pack.pro_line_num = atoi(argv[i]);
            state = 1;
            break;
        default:
            break;
        }
    }
    if(!*g_pack.output_file){
        //默认输出文件名
        snprintf(g_pack.output_file, FILE_NAME_LEN, "%s.out", g_pack.input_file);
    }
    if(!g_pack.pro_line_num){
        //统计文件行数
        printf(">>> 统计输入文件行数 [tip: 可以通过 \"-n\" 参数指定要处理的文件行数].\n");
        g_pack.pro_line_num = Util::stat_line_num(g_pack.input_file);
    }

    //2. 获取可执行程序所在的目录
    g_pack.get_exe_path();

    //3. 加载配置文件
    std::string conf_path(g_pack.exe_path);
    conf_path += "/conf";
    char conf_file[] = "_PROJECTNAME_.conf";
    char conf_file_range[] = "_PROJECTNAME_.conf.range";
    comcfg::Configure cfg;
    int ret = cfg.load(conf_path.c_str(), conf_file, conf_file_range);
    if(ret != 0){
        fprintf(stderr, "load_conf fail [conf_path:%s] [conf_file:%s] [range_file:%s].\n",
                conf_path.c_str(), conf_file, conf_file_range);
        exit(-1);
    }

    //4. 读取配置，初始化g_pack相关字段
    g_pack.init(cfg);

    //5. 读取日志配置，并打开日志
    char log_path[1024];
    char log_file[1024];
    uint32_t log_level;
    uint32_t log_size;
    snprintf(log_path, 1024, "%s/%s", g_pack.exe_path, cfg["log_path"].to_cstr());
    strncpy(log_file, cfg["log_file"].to_cstr(), 1024);
    log_level = cfg["log_level"].to_uint32();
    log_size = cfg["log_size"].to_uint32();
    ret = ub_log_init(log_path, log_file, log_size, log_level);
    if(ret != 0){
        fprintf(stderr, "open log fail.\n");
        exit(-1);
    }

    //6. 初始化数据输入模块
    if(0 != data_input.init(cfg)){
        UB_LOG_FATAL("init data_input fail.");
        exit(-1);
    }

    //7. 初始化数据输出模块
    if(0 != data_output.init(cfg)){
        UB_LOG_FATAL("init data_output fail.");
        exit(-1);
    }

    //8. 初始化数据处理模块
    if(0 != data_proc.init(cfg)){
        UB_LOG_FATAL("init data_proc fail.");
        exit(-1);
    }
}/*}}}*/


/** 主程序 */
int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);

    //1. 解析命令行参数，读取配置并初始化
    parse_args(argc, argv);
    g_pack.output();


    //2. 初始化互斥锁
    int ret = pthread_mutex_init(&g_pack.block_lock, NULL);
    if(ret != 0){
        UB_LOG_FATAL("pthread_mutex_init pool_lock fail.");
        exit(-1);
    }

    //3. 启动各个模块
    data_input.run(&g_pack);
    data_output.run(&g_pack);
    data_proc.run(&g_pack);

    //4. 等待结束
    data_input.join();
    data_output.join();
    data_proc.join();

    printf("\n>>> Done. [output_file: %s]\n", g_pack.output_file);

    return 0;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
