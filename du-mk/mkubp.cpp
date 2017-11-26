/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: newsh.cpp, v1.0.0, 2015-12-30 10:59:31, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   mktoolp.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:59:31
 * @brief  mktoolp.cpp
 **/

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <vector>
#include "du.h"
#include "string.h"
using namespace std;

vector<string> modle_list;

/* 打印帮助信息 */
void print_usage(const char *p)
{
    printf("[usage]: %s [module1 module2 ...]\n", p);
    printf("e.g.\n");
    printf("\t%s\n", p);
    printf("\t%s proxy keyword user\n", p);
}

/** 生成common中的文件 */
void generate_common(du_t &du)
{/*{{{*/
    char srcf[2048];
    char desf[2048];
    snprintf(srcf, 2048, "%s/mkubp/common/Makefile", du.temp_path);
    snprintf(desf, 2048, "common/Makefile");
    gen_file(srcf, desf, du);
    //3. generate def.h file
    snprintf(srcf, 2048, "%s/mkubp/common/def.h", du.temp_path);
    snprintf(desf, 2048, "common/%s_def.h", du._FILENAME_);
    gen_file(srcf, desf, du);
    //4. generate auto_res.h file
    snprintf(srcf, 2048, "%s/mkubp/common/auto_res.h", du.temp_path);
    snprintf(desf, 2048, "common/auto_res.h");
    gen_file(srcf, desf, du);
    //5. generate idl and idl_util.cpp file
    snprintf(srcf, 2048, "%s/mkubp/common/interface.idl", du.temp_path);
    snprintf(desf, 2048, "common/%s_interface.idl", du._FILENAME_);
    gen_file(srcf, desf, du);
    snprintf(srcf, 2048, "%s/mkubp/common/idl_util.cpp", du.temp_path);
    snprintf(desf, 2048, "common/idl_util.cpp");
    gen_file(srcf, desf, du);
    //6. generate func.h/.cpp file
    snprintf(srcf, 2048, "%s/mkubp/common/func.h", du.temp_path);
    snprintf(desf, 2048, "common/%s_func.h", du._FILENAME_);
    gen_file(srcf, desf, du);
    snprintf(srcf, 2048, "%s/mkubp/common/func.cpp", du.temp_path);
    snprintf(desf, 2048, "common/%s_func.cpp", du._FILENAME_);
    gen_file(srcf, desf, du);
    //7. generate ubsvr.h and ubsvr_impl.h
    snprintf(srcf, 2048, "%s/mkubp/common/ubsvr.h", du.temp_path);
    snprintf(desf, 2048, "common/ubsvr.h");
    gen_file(srcf, desf, du);
    snprintf(srcf, 2048, "%s/mkubp/common/ubsvr_impl.h", du.temp_path);
    snprintf(desf, 2048, "common/ubsvr_impl.h");
    gen_file(srcf, desf, du);
    //8. generate other.h/.cpp
    snprintf(srcf, 2048, "%s/mkubp/common/filemonitor.h", du.temp_path);
    snprintf(desf, 2048, "common/filemonitor.h");
    gen_file(srcf, desf, du);
    snprintf(srcf, 2048, "%s/mkubp/common/filemonitor.cpp", du.temp_path);
    snprintf(desf, 2048, "common/filemonitor.cpp");
    gen_file(srcf, desf, du);
    snprintf(srcf, 2048, "%s/mkubp/common/fileout.h", du.temp_path);
    snprintf(desf, 2048, "common/fileout.h");
    gen_file(srcf, desf, du);
    snprintf(srcf, 2048, "%s/mkubp/common/fileout.cpp", du.temp_path);
    snprintf(desf, 2048, "common/fileout.cpp");
    gen_file(srcf, desf, du);
}/*}}}*/

/** 生成模块中的文件 */
void generate_model(string module_name, du_t &du)
{/*{{{*/
    char srcf[2048];
    char desf[2048];
    string c_module_name = module_name;
    c_module_name[0] -= 32;
    char UPMNAME[1024];
    snprintf(UPMNAME, 1024, "%s", module_name.c_str());
    upper(UPMNAME);
    //1. set sed
    string sed;
    sed = sed+"s/_NAMESPACE_/"+du._FILENAME_+"/g; s/_WORKROOT_/"+du._WORK_ROOT_+"/g;";
    sed = sed+"s/__DATETIME__/"+du._DATETIME_+"/g;";
    sed = sed+"s/__YEAR__/"+du._YEAR_+"/g;";
    sed = sed+"s/_NAMESPACE_/"+du._FILENAME_+"/g;";
    sed = sed+"s/_UPNAMESPACE_/"+du._HEAD_FILENAME_+"/g;";
    sed = sed+"s/_MODULE_NAME_/"+module_name+"/g;";
    sed = sed+"s/_KMODULE_NAME_/"+c_module_name+"/g;";
    sed = sed+"s/__UPNAME__/"+UPMNAME+"/g;";
    //2. generate Makefile
    snprintf(srcf, 2048, "%s/mkubp/module/Makefile", du.temp_path);
    snprintf(desf, 2048, "%s/Makefile", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    //3. generate main.cpp
    snprintf(srcf, 2048, "%s/mkubp/module/main.cpp", du.temp_path);
    snprintf(desf, 2048, "%s/main.cpp", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    //4. generate conf
    string cmd = "mkdir ";
    cmd += module_name+"/conf";
    system(cmd.c_str());
    snprintf(srcf, 2048, "%s/mkubp/module/module.conf", du.temp_path);
    snprintf(desf, 2048, "%s/conf/%s.conf", module_name.c_str(), module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    snprintf(srcf, 2048, "%s/mkubp/module/module.conf.range", du.temp_path);
    snprintf(desf, 2048, "%s/conf/%s.conf.range", module_name.c_str(), module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    //5. generate modulesvr.h and modulesvr.cpp
    snprintf(srcf, 2048, "%s/mkubp/module/module.h", du.temp_path);
    snprintf(desf, 2048, "%s/%ssvr.h", module_name.c_str(), module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    snprintf(srcf, 2048, "%s/mkubp/module/module.cpp", du.temp_path);
    snprintf(desf, 2048, "%s/%ssvr.cpp", module_name.c_str(), module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    //6. generate svrcallback.h and svrcallback.cpp
    snprintf(srcf, 2048, "%s/mkubp/module/svrcallback.h", du.temp_path);
    snprintf(desf, 2048, "%s/svrcallback.h", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    snprintf(srcf, 2048, "%s/mkubp/module/svrcallback.cpp", du.temp_path);
    snprintf(desf, 2048, "%s/svrcallback.cpp", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    //7. generate session.h and session.cpp
    snprintf(srcf, 2048, "%s/mkubp/module/session.h", du.temp_path);
    snprintf(desf, 2048, "%s/session.h", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    snprintf(srcf, 2048, "%s/mkubp/module/session.cpp", du.temp_path);
    snprintf(desf, 2048, "%s/session.cpp", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    //8. generate proc.h
    snprintf(srcf, 2048, "%s/mkubp/module/proc.h", du.temp_path);
    snprintf(desf, 2048, "%s/proc.h", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    //9. procs dir and main_proc.h main_proc.cpp
    cmd = "mkdir ";
    cmd += module_name+"/procs";
    system(cmd.c_str());
    snprintf(srcf, 2048, "%s/mkubp/module/main_proc.h", du.temp_path);
    snprintf(desf, 2048, "%s/procs/main_proc.h", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
    snprintf(srcf, 2048, "%s/mkubp/module/main_proc.cpp", du.temp_path);
    snprintf(desf, 2048, "%s/procs/main_proc.cpp", module_name.c_str());
    exe_sed(srcf, desf, sed.c_str());
}/*}}}*/

/** 主程序 */
int main(int argc, char *argv[])
{
    du_t du;
    parse_args(argc, argv, du);

    string modle;
    if(argc > 1){
        for(int i=1; i<argc; ++i){
            modle = argv[i];
            modle_list.push_back(modle);
        }
    }else{
        modle = du._FILENAME_;
        modle_list.push_back(modle);
    }

    // 检查是否在svn_root中
    if (!check_work_root(du.cur_path, du._WORK_ROOT_)){
        print_exe("check baidu_svn_root fail: This directory is not in baidu svn root.");
        return -1;
    }
    strcat(du._WORK_ROOT_, "\\/..");

    // 创建目录
    string cmd;
    cmd = "mkdir common";
    system(cmd.c_str());
    print_exe(cmd.c_str());
    vector<string>::iterator it = modle_list.begin();
    for(it=modle_list.begin(); it!=modle_list.end(); ++it){
        cmd = "mkdir ";
        cmd += *it;
        print_exe(cmd.c_str());
        system(cmd.c_str());
    }

    // 生成common中的文件
    generate_common(du);

    // 生成各个模块中的文件
    for(it=modle_list.begin(); it!=modle_list.end(); ++it){
        string msg = "generate ";
        msg += *it;
        msg += " files";
        print_exe(msg.c_str());
        generate_model(*it, du);
    }

    // 生成最外层的Makefile文件
    string ms;
    for(it=modle_list.begin(); it!=modle_list.end(); ++it){
        ms = ms + " " + *it;
    }
    string msg;
    msg = msg+".PHONY: all clean common "+ms+"\n\n";
    msg = msg+"all:clean common "+ms+"\n";
    msg = msg+"\tmkdir -p output/common\n";
    for(it=modle_list.begin(); it!=modle_list.end(); ++it){
        msg = msg+"\tmkdir -p output/"+*it+"\n";
    }
    msg = msg+"\tcp -rf ./common/output/* ./output/common/\n";
    for(it=modle_list.begin(); it!=modle_list.end(); ++it){
        msg = msg+"\tcp -rf ./"+*it+"/output/* ./output/"+*it+"/\n";
    }
    msg = msg+"\ncommon:\n\tmake -C ./common\n";
    for(it=modle_list.begin(); it!=modle_list.end(); ++it){
        msg = msg+"\n"+*it+":\n\tmake -C ./"+*it+"\n";
    }
    msg = msg+"\nclean:\n";
    msg = msg+"\tmake -C ./common clean\n";
    for(it=modle_list.begin(); it!=modle_list.end(); ++it){
        msg = msg+"\tmake -C ./"+*it+" clean\n";
    }
    msg = msg+"\trm -rf output\n\n";
    FILE *fp;
    fp = fopen("Makefile", "w");
    if(!fp){
        fprintf(stderr, "can not open Makefile\n");
        return -1;
    }
    fprintf(fp, "%s", msg.c_str());
    fclose(fp);
    print_exe("generate Makefile");

    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
