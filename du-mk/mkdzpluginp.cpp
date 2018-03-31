/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2015-12-30 10:59:31, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   mkdzpluginp.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:59:31
 * @brief  mkdzpluginp.cpp
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
    exe_cmd("mkdir -p api/1;touch api/index.htm;touch api/1/index.htm");
    exe_cmd("mkdir class;touch class/index.htm");
    exe_cmd("mkdir model;touch model/index.htm");
    exe_cmd("mkdir table;touch table/index.htm");

    //2. copy files
    const int m=3;
    char copy_files[m][1024] = {"template","build.js","r.js"};
    for (int i=0;i<m;++i) {
        snprintf(tempfile, 1024, "cp -r %s/mkdzpluginp/%s %s", du.temp_path, copy_files[i], copy_files[i]);
        exe_cmd(tempfile);
    }
    exe_cmd("rm template/*.htm template/views/*.tpl");

    //3. copy and sed files
    const int n=31;
    char files[n][1024] = {
        "README.md",
        "build.sh","index.php",
        "plugin.inc.php","install.php","uninstall.php",
        "check.php","upgrade.php",
        "hook_mobile.class.php","hook_pc.class.php",
        "z_nav.inc.php","z_setting.inc.php","z_auth.inc.php","z_about.inc.php",

        "api/1/seccode.php","api/1/uc.php","api/1/admin.php","api/1/excel.php","api/1/dict.php",
        "class/env.class.php","class/log.class.php","class/utils.class.php","class/validate.class.php",
        "template/header.htm","template/footer.htm","template/admin.htm","template/Error.htm",
        "template/views/z_nav.tpl",
        "template/views/z_setting.tpl",
        "template/views/z_auth.tpl",
        "template/views/z_about.tpl"
    };
    for (int i=0;i<n;++i) {
        snprintf(tempfile, 1024, "%s/mkdzpluginp/%s", du.temp_path, files[i]);
        snprintf(destfile, 1024, "%s", files[i]);
        gen_file(tempfile, destfile, du);
    }

    //4. copy and sed and rename files
    const int k=11;
    char csr_files[k*2][1024] = {
        "discuz_plugin.xml","discuz_plugin_%s.xml",
        "datatool.inc.php", "%s.inc.php",
        "admin.inc.php", "admin.inc.php",
        "template/datatool.htm","template/%s.htm",
        "model/model_nav_setting.php", "model/model_%s_nav_setting.php",
        "model/model_seccode.php","model/model_%s_seccode.php",
        "model/model_setting.php","model/model_%s_setting.php",
        "model/model_uc.php","model/model_%s_uc.php",
        "model/model_usergroup.php","model/model_%s_usergroup.php",
        "table/table_auth.php","table/table_%s_auth.php",
        "table/table_log.php","table/table_%s_log.php"
    };
    for (int i=0;i<k*2;++i) {
        snprintf(tempfile, 1024, "%s/mkdzpluginp/%s", du.temp_path, csr_files[i]);
        snprintf(destfile, 1024, csr_files[++i], du._FILENAME_);
//        printf("gen_file %s %s\n",tempfile,destfile);
        gen_file(tempfile, destfile, du, true);
    }

    printf("You can use this command to remove all .svn:\n\tfind . -type d -name \".svn\" | xargs rm -rf\n");

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
