/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2015-12-30 10:59:31, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   mktoolexp.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:59:31
 * @brief  mktoolexp.cpp
 **/

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include "du.h"
using namespace std;

/* 打印帮助信息 */
void print_usage(const char *p)
{
    printf("[usage]: %s\n", p);
}

/** 主程序 */
int main(int argc, char *argv[])
{
    du_t du;
    parse_args(argc, argv, du);

    // 检查是否在svn_root中
    if (!check_work_root(du.cur_path, du._WORK_ROOT_)){
        print_exe("check baidu_svn_root fail: This directory is not in baidu svn root.");
        return -1;
    }

    char tempfile[1024];
    char destfile[1024];
    snprintf(tempfile, 1024, "%s/mktoolexp/main.cpp", du.temp_path);
    snprintf(destfile, 1024, "main.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/util.h", du.temp_path);
    snprintf(destfile, 1024, "util.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/util.cpp", du.temp_path);
    snprintf(destfile, 1024, "util.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/g_pack_t.h", du.temp_path);
    snprintf(destfile, 1024, "g_pack_t.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/g_pack_t.cpp", du.temp_path);
    snprintf(destfile, 1024, "g_pack_t.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/ubclient.h", du.temp_path);
    snprintf(destfile, 1024, "ubclient.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/ubclient.cpp", du.temp_path);
    snprintf(destfile, 1024, "ubclient.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/ubclient.h", du.temp_path);
    snprintf(destfile, 1024, "ubclient.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/ubclient.cpp", du.temp_path);
    snprintf(destfile, 1024, "ubclient.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/data_input.h", du.temp_path);
    snprintf(destfile, 1024, "data_input.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/data_input.cpp", du.temp_path);
    snprintf(destfile, 1024, "data_input.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/data_output.h", du.temp_path);
    snprintf(destfile, 1024, "data_output.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/data_output.cpp", du.temp_path);
    snprintf(destfile, 1024, "data_output.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/data_proc.h", du.temp_path);
    snprintf(destfile, 1024, "data_proc.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/data_proc.cpp", du.temp_path);
    snprintf(destfile, 1024, "data_proc.cpp");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/nqc_client.h", du.temp_path);
    snprintf(destfile, 1024, "nqc_client.h");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mktoolexp/nqc_client.cpp", du.temp_path);
    snprintf(destfile, 1024, "nqc_client.cpp");
    gen_file(tempfile, destfile, du);


    snprintf(tempfile, 1024, "%s/mktoolexp/Makefile", du.temp_path);
    snprintf(destfile, 1024, "Makefile");
    gen_file(tempfile, destfile, du);

    exe_cmd("mkdir conf");

    snprintf(tempfile, 1024, "%s/mktoolexp/conf/toolp.conf", du.temp_path);
    snprintf(destfile, 1024, "./conf/%s.conf", du._FILENAME_);
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mktoolexp/conf/toolp.conf.range", du.temp_path);
    snprintf(destfile, 1024, "./conf/%s.conf.range", du._FILENAME_);
    gen_file(tempfile, destfile, du);

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
