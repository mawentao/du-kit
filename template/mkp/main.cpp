/***************************************************************************
 *
 * Copyright (c) _YEAR_ _AUTHOR_, Inc. All Rights Reserved
 * $Id: main.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   main.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  主程序入口
 **/

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;


/** 解析命令行参数，读取配置并初始化 */
int parse_args(int argc, char *argv[])
{

    return 0;
}


/** 主程序 */
int main(int argc, char *argv[])
{

    //1. 解析命令行参数，读取配置并初始化
    int ret = parse_args(argc, argv);
    if(0 != ret){
        printf("parse_args fail.");
    }

    cout<<"!!!THE END!!!"<<endl;
    return 0;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
