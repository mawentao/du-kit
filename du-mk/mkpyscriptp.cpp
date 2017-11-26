/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2015-12-30 10:59:31, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   mkpyscriptp.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:59:31
 * @brief  mkpyscriptp.cpp
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

    char tempfile[1024];
    char destfile[1024];
    snprintf(tempfile, 1024, "%s/mkpyscriptp/run.py", du.temp_path);
    snprintf(destfile, 1024, "run.py");
    gen_file(tempfile, destfile, du);

    exe_cmd("mkdir scripts");
    snprintf(tempfile, 1024, "%s/mkpyscriptp/scripts/common.py", du.temp_path);
    snprintf(destfile, 1024, "scripts/common.py");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mkpyscriptp/scripts/Crawl.py", du.temp_path);
    snprintf(destfile, 1024, "scripts/Crawl.py");
    gen_file(tempfile, destfile, du);
    snprintf(tempfile, 1024, "%s/mkpyscriptp/scripts/__init__.py", du.temp_path);
    snprintf(destfile, 1024, "scripts/__init__.py");
    gen_file(tempfile, destfile, du);

    exe_cmd("mkdir conf");
    snprintf(tempfile, 1024, "%s/mkpyscriptp/conf/main.ini", du.temp_path);
    snprintf(destfile, 1024, "conf/main.ini");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "cp -r %s/mkpyscriptp/mtlib .", du.temp_path);
    exe_cmd(tempfile);
    snprintf(tempfile, 1024, "cp -r %s/mkpyscriptp/xpath .", du.temp_path);
    exe_cmd(tempfile);

    printf("You can use this command to remove all .svn:\n\tfind . -type d -name \".svn\" | xargs rm -rf\n");

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
