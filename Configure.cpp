/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: Configure.cpp, v1.0.0, 2015-12-30 09:37:17, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   Configure.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 09:37:17
 * @brief  Configure.cpp
 **/

#include "Configure.h"

bool is_key_char(char c)
{
    if ( (c<='z' && c>='a') || (c<='Z' && c>='A') ||
         (c<='9' && c>='0') || c=='_' || c=='-'
    ) {
        return true;
    }
    return false;
}

bool is_value_char(char c)
{
    if (is_key_char(c)) return true;
    if (c=='.' || c=='/' || c=='\\' || c==',' || c=='?' || c=='|' || c=='<' || c=='>' || c=='(' || c==')' ||
        c=='{' || c=='}' || c=='*' || c=='$' || c=='&' || c=='^' || c=='@'
    ) return true;
    return false;
}

int Configure::load(const char *conf_path, const char *conf_file)
{
    //1. 打开配置文件
    char conffile[MAX_FILE_PATH_LEN];
    snprintf(conffile, MAX_FILE_PATH_LEN, "%s/%s", conf_path, conf_file);
    FILE *fi = fopen(conffile, "r");
    if(!fi){
        fprintf(stderr, "[FATAL] can not open file [%s]\n", conffile);
        exit(-1);
    }

    //2. 解析配置文件
    char c;
    int state=0;
    char kbuf[MAX_FILE_PATH_LEN];
    char vbuf[MAX_FILE_PATH_LEN];
    char *p=kbuf;
    while ( (c=fgetc(fi)) != EOF ) {
        switch(state) {
            case 0:
                if (c=='#') state=1;
                else if (is_key_char(c)) {
                    *p++ = c;
                    state=2;
                }
                break;
            case 1:
                if (c=='\n') state=0;
                break;
            case 2:
                if (is_key_char(c)) *p++ = c;
                else if (c=='\n') {
                    p=kbuf;
                    state=0;
                }
                else if (c=='#') {
                    p=kbuf;
                    state=1;
                }
                else if (c==':') {
                    *p='\0';
                    p = vbuf;
                    state=3;
                }
                break;
            case 3:
                if (is_value_char(c)) *p++ = c;
                else if (c=='#' || c=='\n') {
                    *p='\0';
                    //printf("%s : %s\n", kbuf, vbuf);
                    map[kbuf] = vbuf;
                    p=kbuf;
                    if (c=='#') state=1;
                    if (c=='\n') state=0;
                }
                break;
        }
    }

    /* for debug
    CfgMap::iterator it;
    for (it=map.begin(); it!=map.end(); ++it) {
        printf("%s : %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("load %s\n", conffile);
    */
    return 0;
}





// vim:fdm=marker:nu:ts=4:sw=4:expandtab
