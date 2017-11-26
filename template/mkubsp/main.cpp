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

#include <iostream>
#include <sys/time.h>
#include "svrcallback.h"
using namespace std;

//////////////////////////////////////////////////////

//! 全局配置变量
conf_t g_conf;

//////////////////////////////////////////////////////



/** server_idl_t exception 回调函数 **/
static void exception_handler(bsl::Exception& e)
{
//    UB_LOG_WARNING(e);
    return ;
}


/** 读取配置，初始化Sunflower和Webkit客户端 */
static int init(const char *conf_path, const char *conf_file, const char *conf_file_range)
{/*{{{*/
    //1. 打开配置文件
    comcfg::Configure conf;
    int ret = conf.load(conf_path, conf_file, conf_file_range);
    if(ret != 0){
        fprintf(stderr, "load_conf fail [conf_path:%s] [conf_file:%s] [range_file:%s].\n",
                conf_path, conf_file, conf_file_range);
        return FUN_FAIL;
    }

    //2. 读取日志配置，打开日志
    char log_path[1024];
    char log_file[1024];
    uint32_t log_level;
    uint32_t log_size;
    snprintf(log_path, 1024, "%s", conf["LOG_PATH"].to_cstr());
    strncpy(log_file, conf["LOG_NAME"].to_cstr(), 1024);
    log_size  = conf["LOG_SIZE"].to_uint32();
    log_level = conf["LOG_LEVEL"].to_uint32();
    ret = ub_log_init(log_path, log_file, log_size, log_level);
    if(ret != 0){
        fprintf(stderr, "open log fail.\n");
        return FUN_FAIL;
    }

    //3. 读取UBServer&初始化
    comcfg::ConfigUnit &cfg = conf["UBServer"];
    snprintf(g_conf.svr_conf.svr_name, sizeof(g_conf.svr_conf.svr_name), "%s", cfg["name"].to_cstr());
    g_conf.svr_conf.port = cfg["port"].to_uint32();
    g_conf.svr_conf.thread_num      = cfg["threadnum"].to_uint32();
    g_conf.svr_conf.connect_type    = cfg["connecttype"].to_uint32();
    g_conf.svr_conf.server_type     = cfg["servertype"].to_uint32();
    g_conf.svr_conf.read_timeout    = cfg["read_timeout"].to_uint32();
    g_conf.svr_conf.write_timeout   = cfg["write_timeout"].to_uint32();
    g_conf.svr_conf.connect_timeout = cfg["connect_timeout"].to_uint32();
    g_conf.svr_conf.read_buf_size   = cfg["read_buf_size"].to_uint32();
    g_conf.svr_conf.write_buf_size  = cfg["write_buf_size"].to_uint32();

    UB_LOG_TRACE(" [name:%s] [port:%u] [threadnum:%u] [connecttype:%u] [servertype:%u]"
                 " [read_timeout:%u] [write_timeout:%u] [connect_timeout:%u]"
                 " [read_buf_size:%u] [write_buf_size:%u]",
                 g_conf.svr_conf.svr_name,
                 g_conf.svr_conf.port,
                 g_conf.svr_conf.thread_num,
                 g_conf.svr_conf.connect_type,
                 g_conf.svr_conf.server_type,
                 g_conf.svr_conf.read_timeout,
                 g_conf.svr_conf.write_timeout,
                 g_conf.svr_conf.connect_timeout,
                 g_conf.svr_conf.read_buf_size,
                 g_conf.svr_conf.write_buf_size);

    g_conf.read_buf_size  = g_conf.svr_conf.read_buf_size;
    g_conf.write_buf_size = g_conf.svr_conf.write_buf_size;
    g_conf.tmp_buf_size   = cfg["tmp_buf_size"].to_uint32();

    return FUN_SUCC;
}/*}}}*/

/**
 * @brief 主函数
 * @param argc
 * @param argv[]
 * @return
 */
int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);

    //1. 解析命令行参数&读取配置
    if(FUN_SUCC != init("./conf", "_PROJECTNAME_.conf", "_PROJECTNAME_.conf.range")){
        UB_LOG_FATAL("init fail. [conf_path:./conf]");
        suicide();
    }
    UB_LOG_TRACE("init success.");
    g_conf.output();

    //2. 创建服务对象
    server_idl_t svr(g_conf.svr_conf,
            g_conf.read_buf_size, g_conf.write_buf_size,
            g_conf.tmp_buf_size, g_conf.tmp_buf_size,
            NULL, exception_handler);
/*
    g_conf.output();
*/
    //3. 启动服务
    svr.run(svr_callback);
    printf("_PROJECTNAME_ server is started. [port:%u]\n", g_conf.svr_conf.port);
    UB_LOG_TRACE("_PROJECTNAME_ server is started. [port:%u]\n", g_conf.svr_conf.port);

    //4. 等待结束
    svr.join();

    return 0;
}



// vim:fdm=marker:nu:ts=4:sw=4:expandtab
