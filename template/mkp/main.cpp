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
 * @brief  ���������
 **/

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;


/** ���������в�������ȡ���ò���ʼ�� */
int parse_args(int argc, char *argv[])
{

    return 0;
}


/** ������ */
int main(int argc, char *argv[])
{

    //1. ���������в�������ȡ���ò���ʼ��
    int ret = parse_args(argc, argv);
    if(0 != ret){
        printf("parse_args fail.");
    }

    cout<<"!!!THE END!!!"<<endl;
    return 0;
}


// vim:fdm=marker:nu:ts=4:sw=4:expandtab
