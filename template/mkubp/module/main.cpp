/***************************************************************************
 *
 * Copyright (c) __YEAR__ Baidu.com, Inc. All Rights Reserved
 * $Id: main.cpp, v1.0.0, __DATETIME__, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   main.cpp
 * @author mawt(mawentao@baidu.com)
 * @date   __DATETIME__
 * @brief  main.cpp
 **/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "_MODULE_NAME_svr.h"
using namespace std;
using namespace _NAMESPACE_;

////////////////ȫ�ֱ�������////////////////////////

_KMODULE_NAME_Svr g__MODULE_NAME__svr; /**< ȫ�ֶ��� */

////////////////////////////////////////////////////


/** �źŴ����� */
static void signal_handler(int sig)
{/*{{{*/
    g__MODULE_NAME__svr.m_run_flag = false;
}/*}}}*/

/** �����źŴ����� */
static void signal_setup()
{/*{{{*/
    signal(SIGPIPE, SIG_IGN);
	signal(SIGINT,  signal_handler);
	signal(SIGTERM,  signal_handler);
}/*}}}*/


/** ���������в�������ȡ���ò���ʼ�� */
static int parse_args(int argc, char *argv[])
{/*{{{*/
    //1. load conf file
    char conf_path[] = "./conf";
    char conf_file[] = "_MODULE_NAME_.conf";
    char conf_file_range[] = "_MODULE_NAME_.conf.range";
    comcfg::Configure cfg;
    int ret = cfg.load(conf_path, conf_file, conf_file_range);
    if(ret != 0){
        fprintf(stderr, "load conf fail. [path:%s] [conf:%s] [range:%s]\n",
                conf_path, conf_file, conf_file_range);
        return _NAMESPACE_::FAIL;
    }

    //2. ��ȡ��־���ã�������־
    char log_path[1024];
    char log_file[1024];
    uint32_t log_level;
    uint32_t log_size;
    strncpy(log_path, cfg["log_path"].to_cstr(), 1024);
    strncpy(log_file, cfg["log_file"].to_cstr(), 1024);
    log_level = cfg["log_level"].to_uint32();
    log_size = cfg["log_size"].to_uint32();
    ret = ub_log_init(log_path, log_file, log_size, log_level);
    if(0 != ret){
        fprintf(stderr, "open log fail.\n");
        return _NAMESPACE_::FAIL;
    }

    //3. ��ʼ��ȫ�ֶ���
    ret = g__MODULE_NAME__svr.init(cfg);
    if(_NAMESPACE_::FAIL == ret){
        UB_LOG_FATAL("init g__MODULE_NAME__svr fail.");
        _NAMESPACE_::suicide();
    }

    printf("server is running on port [ %d ]\n\n", cfg["_MODULE_NAME_"]["port"].to_uint32());

    return _NAMESPACE_::SUCC;
}/*}}}*/


/** ��������� */
int main(int argc, char *argv[])
{
    //1. ���������в�������ȡ���ò���ʼ��
    int ret = parse_args(argc, argv);
    if(_NAMESPACE_::FAIL == ret){
        fprintf(stderr, "parse_args fail.\n");
        _NAMESPACE_::suicide();
    }

    //2. �����ź���
    signal_setup();

    //3. ��������
    ret = g__MODULE_NAME__svr.start();
    if(_NAMESPACE_::FAIL == ret){
        fprintf(stderr, "start _MODULE_NAME__server fail.\n");
        _NAMESPACE_::suicide();
    }

    //4. ���߳�˯�ߵȴ�
    while(g__MODULE_NAME__svr.m_run_flag){
        sleep(1);
    }

    //5. �˳�
    g__MODULE_NAME__svr.stop();
    _NAMESPACE_::suicide();

    return 0;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
