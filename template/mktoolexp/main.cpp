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
 * @brief  ���������
 **/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "data_input.h"
#include "data_output.h"
#include "data_proc.h"
using namespace std;


//////////////////////////////////////////////

/* ȫ���ڴ� */
g_pack_t g_pack;

/* ��������ģ�� */
DataInput data_input;

/* �������ģ�� */
DataOutput data_output;

/* ���ݴ���ģ�� */
DataProc data_proc;

//////////////////////////////////////////////


/** ��ӡ���� */
void print_usage(const char *pname)
{/*{{{*/
    printf("toolp v1.0 (mawentao@baidu.com)\n");
    printf("\n[usage]:\n");
    printf("    %s input_file [-o output_file]\n", pname);
}/*}}}*/

/** ���������в�������ȡ���ò���ʼ�� */
void parse_args(int argc, char *argv[])
{/*{{{*/
    //1. ���������в���
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
        //Ĭ������ļ���
        snprintf(g_pack.output_file, FILE_NAME_LEN, "%s.out", g_pack.input_file);
    }
    if(!g_pack.pro_line_num){
        //ͳ���ļ�����
        printf(">>> ͳ�������ļ����� [tip: ����ͨ�� \"-n\" ����ָ��Ҫ������ļ�����].\n");
        g_pack.pro_line_num = Util::stat_line_num(g_pack.input_file);
    }

    //2. ��ȡ��ִ�г������ڵ�Ŀ¼
    g_pack.get_exe_path();

    //3. ���������ļ�
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

    //4. ��ȡ���ã���ʼ��g_pack����ֶ�
    g_pack.init(cfg);

    //5. ��ȡ��־���ã�������־
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

    //6. ��ʼ����������ģ��
    if(0 != data_input.init(cfg)){
        UB_LOG_FATAL("init data_input fail.");
        exit(-1);
    }

    //7. ��ʼ���������ģ��
    if(0 != data_output.init(cfg)){
        UB_LOG_FATAL("init data_output fail.");
        exit(-1);
    }

    //8. ��ʼ�����ݴ���ģ��
    if(0 != data_proc.init(cfg)){
        UB_LOG_FATAL("init data_proc fail.");
        exit(-1);
    }
}/*}}}*/


/** ������ */
int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);

    //1. ���������в�������ȡ���ò���ʼ��
    parse_args(argc, argv);
    g_pack.output();


    //2. ��ʼ��������
    int ret = pthread_mutex_init(&g_pack.block_lock, NULL);
    if(ret != 0){
        UB_LOG_FATAL("pthread_mutex_init pool_lock fail.");
        exit(-1);
    }

    //3. ��������ģ��
    data_input.run(&g_pack);
    data_output.run(&g_pack);
    data_proc.run(&g_pack);

    //4. �ȴ�����
    data_input.join();
    data_output.join();
    data_proc.join();

    printf("\n>>> Done. [output_file: %s]\n", g_pack.output_file);

    return 0;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
