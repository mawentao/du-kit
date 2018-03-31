/***************************************************************************
 *
 * Copyright (c) 2017 mawentao.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2017-12-21 10:59:31, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   newppt.cpp
 * @author mawentao
 * @date   2017-12-21 10:59:31
 * @brief  newppt.cpp
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
    printf("[usage]: %s style filename\ne.g.\n\t%s didi myppt\n", p, p);
    exit(0);
}

/** 主程序 */
int main(int argc, char *argv[])
{
    du_t du;
    parse_args(argc, argv, du);
    if (argc<3) { print_usage(argv[0]); }

    char tempfile[1024];
    char destfile[1024];
    snprintf(tempfile, 1024, "%s/ppt/%s.pptx", du.temp_path, argv[1]);
    snprintf(destfile, 1024, "%s.pptx", trip_suffix(argv[2]));

    if (!is_file(tempfile)) {
        printf(">>> [Error]: can not find template file: %s\n", tempfile);
        print_usage(argv[0]);
    }

    //printf("argc: %d\ncp %s %s\n", argc, tempfile, destfile);
    copy_file(tempfile, destfile);

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
