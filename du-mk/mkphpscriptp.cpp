/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2015-12-30 10:59:31, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   mkphpscriptp.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:59:31
 * @brief  mkphpscriptp.cpp
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
    snprintf(tempfile, 1024, "%s/mkphpscriptp/run.php", du.temp_path);
    snprintf(destfile, 1024, "run.php");
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/mkphpscriptp/build.sh", du.temp_path);
    snprintf(destfile, 1024, "build.sh");
    gen_file(tempfile, destfile, du);

    exe_cmd("mkdir scripts");
    snprintf(tempfile, 1024, "%s/mkphpscriptp/scripts/Test.php", du.temp_path);
    snprintf(destfile, 1024, "scripts/Test.php");
    gen_file(tempfile, destfile, du);

    exe_cmd("mkdir conf");
    snprintf(tempfile, 1024, "%s/mkphpscriptp/conf/main.ini", du.temp_path);
    snprintf(destfile, 1024, "conf/main.ini");
    gen_file(tempfile, destfile, du);

    exe_cmd("mkdir conf/online");
    snprintf(tempfile, 1024, "%s/mkphpscriptp/conf/online/main.ini", du.temp_path);
    snprintf(destfile, 1024, "conf/online/main.ini");
    gen_file(tempfile, destfile, du);

    exe_cmd("mkdir models");

    snprintf(tempfile, 1024, "cp -r %s/mkphpscriptp/mt .", du.temp_path);
    exe_cmd(tempfile);

    printf("You can use this command to remove all .svn:\n\tfind . -type d -name \".svn\" | xargs rm -rf\n");

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
