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
#include "g_pack_t.h"
#include "ubclient.h"
using namespace std;


//////////////////////////////////////////////

/* 全局内存 */
g_pack_t g_pack;

/* 访问UBServer的客户端 */
UBClient ubc;

//////////////////////////////////////////////


/** 打印帮助 */
void print_usage(const char *pname)
{/*{{{*/
    printf("toolp v1.0 (mawentao@baidu.com)\n");
    printf("\n[usage]:\n");
    printf("    %s input_file [-o output_file] [-n line_num]\n", pname);
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

    //6. 初始化UBClient
    ret = ubc.init(cfg["ubserver"]);
    if(0 != ret){
        fprintf(stderr, "init ubclient fail.\n");
        exit(-1);
    }
}/*}}}*/


/** 主程序 */
int main(int argc, char *argv[])
{
    //1. 解析命令行参数，读取配置并初始化
    parse_args(argc, argv);
    g_pack.output();

    //2. 打开输入输出文件
    FILE *fi = fopen(g_pack.input_file, "r");
    FILE *fo = fopen(g_pack.output_file, "w");
    if(!fi){
        UB_LOG_FATAL("open input file fail.");
        exit(-1);
    }
    if(!fo){
        UB_LOG_FATAL("open output file fail.");
        exit(-1);
    }

    //3. 处理文件
    uint32_t cur = 0;    //当前行号
    uint32_t sum = g_pack.pro_line_num;   //总行数
    uint32_t last = 0;   //上次打印进度的行号
    while(fgets(g_pack.line_buf, g_pack.line_buf_size, fi))
    {
        //3-1. 去除行尾空白字符
        Util::str_rtrim(g_pack.line_buf);
        //3-2. 分割列
        g_pack.split_line('\t');

        //访问ubserver
        ubc.do_process();

        //3-3. 处理每一列字段
        for(uint32_t i=0; i<g_pack.col_num; ++i){
            fprintf(fo, "%s\t", g_pack.cols[i]);
        }
        fprintf(fo, "%s\n", g_pack.line_buf);

        //打印进度
        ++cur;
        Util::print_progress(cur, sum, last);
        if(cur >= g_pack.pro_line_num){
            break;
        }
    }

    //4. 关闭退出
    fclose(fi);
    fclose(fo);
    printf("\n>>> Done. [output_file: %s]\n", g_pack.output_file);

    return 0;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
