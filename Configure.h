/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: Configure.h, v1.0.0, 2015-12-30 09:37:17, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   Configure.h
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 09:37:17
 * @brief  Configure.h
 **/

#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>

#define MAX_FILE_PATH_LEN 1024

typedef std::map<std::string, std::string> CfgMap;

class Configure{
public:
    Configure(){}

    Configure(const Configure&);
    Configure& operator=(const Configure&);

    ~Configure(){}

    int load(const char *conf_path, const char *conf_file);

    std::string getstr(const char *key)
    {
        if (map.end() != map.find(key)) return map[key];
        else return "";
    }

private:
    //char **data[MAX_FILE_PATH_LEN];
    CfgMap map;
};


#endif
