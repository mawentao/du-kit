/***************************************************************************
 *
 * Copyright (c) 2017 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2017-09-29 10:59:31, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   mk_java_maven_p.cpp
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
    exe_cmd("mkdir -p src/main/java");
    exe_cmd("mkdir -p src/main/resources");
    exe_cmd("mkdir -p src/test/java");
    exe_cmd("mkdir -p src/test/resources");
    exe_cmd("mkdir -p src/main/java/app/main");
    exe_cmd("mkdir -p src/main/java/app/util");
    exe_cmd("mkdir -p src/main/java/app/bean");
    exe_cmd("mkdir -p src/main/java/app/dao");

    exe_cmd("mkdir -p target");

    //2. copy files
    const int m=6;
    char copy_files[m][1024] = {
        "pom.xml","build.sh","run.sh",
        "src/main/resources/",
        "src/main/java/app/bean/",
        "src/main/java/app/dao/"
    };
    for (int i=0;i<m;++i) {
        snprintf(tempfile, 1024, "cp -r %s/mkjavamavenp/%s %s", du.temp_path, copy_files[i], copy_files[i]);
        exe_cmd(tempfile);
    }

    //3. copy and sed files
    const int n=3;
    char files[n][1024] = {
        "src/main/java/app/main/Main.java",
        "src/main/java/app/main/MybatisDemoMain.java",
        "src/main/java/app/util/TimeTool.java"
    };
    for (int i=0;i<n;++i) {
        snprintf(tempfile, 1024, "%s/mkjavamavenp/%s", du.temp_path, files[i]);
        snprintf(destfile, 1024, "%s", files[i]);
        gen_file(tempfile, destfile, du);
    }

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
