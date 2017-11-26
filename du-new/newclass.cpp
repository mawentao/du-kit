/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2015-12-30 10:59:31, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   newclass.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:59:31
 * @brief  newclass.cpp
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
    printf("[usage]: %s classname\ne.g.\n\t%s user\n", p, p);
}

/** 主程序 */
int main(int argc, char *argv[])
{
    du_t du;
    parse_args(argc, argv, du);

    char tempfile[1024];
    char destfile[1024];
    snprintf(tempfile, 1024, "%s/new/newclass.h", du.temp_path);
    snprintf(destfile, 1024, "%s.h", du._FILENAME_);
    gen_file(tempfile, destfile, du);

    snprintf(tempfile, 1024, "%s/new/newclass.cpp", du.temp_path);
    snprintf(destfile, 1024, "%s.cpp", du._FILENAME_);
    gen_file(tempfile, destfile, du);

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
