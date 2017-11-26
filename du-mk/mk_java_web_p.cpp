/***************************************************************************
 *
 * Copyright (c) 2017 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2017-09-29 10:59:31, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   mk_java_web_p.cpp
 * @author mawentao
 * @date   2015-12-30 10:59:31
 * @brief  Java Maven 工程脚手架
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

    //1. mkdirs
    exe_cmd("mkdir -p src/main/java/app/ctrl");
    exe_cmd("mkdir -p src/main/resources");
    exe_cmd("mkdir -p src/main/webapp/WEB-INF");

    //2. copy files
    const int m=2;
    char copy_files[m][1024] = {
        "src/main/webapp/index.jsp",
        "src/main/resources/MANIFEST.MF"
    };
    for (int i=0;i<m;++i) {
        snprintf(tempfile, 1024, "cp -r %s/mkjavawebp/%s %s", du.temp_path, copy_files[i], copy_files[i]);
        exe_cmd(tempfile);
    }
    //3. copy and sed files
    const int n=6;
    char files[n][1024] = {
        "build.sh",
        "pom.xml",
        "src/main/webapp/WEB-INF/web.xml",
        "src/main/resources/spring-main.xml",
        "src/main/resources/spring-mvc.xml",
        "src/main/java/app/ctrl/BaseController.java"
    };
    for (int i=0;i<n;++i) {
        snprintf(tempfile, 1024, "%s/mkjavawebp/%s", du.temp_path, files[i]);
        snprintf(destfile, 1024, "%s", files[i]);
        gen_file(tempfile, destfile, du);
    }

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
