/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id: du.cpp, v1.0.0, 2015-12-30 10:40:13, mawentao(mawt@youzu.com) Exp $
 *
 **************************************************************************/

/**
 * @file   du.cpp
 * @author mawentao(mawt@youzu.com)
 * @date   2015-12-30 10:40:13
 * @brief  du.cpp
 **/

#include "du.h"
#include "string.h"
#include "unistd.h"
#include <sys/stat.h>
#include <getopt.h>

/**
 * @brief 分离全路径文件名的路径和文件名
 * @param fullpath
 * @param path
 * @param path_size
 * @param file
 * @param file_size
 * @return 1 表示成功，0 表示失败
 */
static int split_full_path_file(const char *fullpath, char *path, int path_size, char * file, int file_size)
{/*{{{*/
    int size = 0;
    int split_place = 0;
    const char *p = fullpath;
    while(*p){
        ++size; 
        if('/' == *p++){
            split_place = size; 
        }       
    }
    int need_path_size = split_place + 1;
    int need_file_size = size - split_place + 1;
    if(need_path_size > path_size || need_file_size > file_size){
        return 0;
    }
    memset(path, 0, path_size);
    memset(file, 0, file_size);
    strncpy(path, fullpath, split_place);
    strncpy(file, fullpath+split_place, need_file_size-1);
    return 1;
}/*}}}*/

/**
 * @brief 获取可执行程序的路径
 * @param buf
 * @param size
 * @return 0 表示成功，-1表示失败
 */
static int get_exe_path(char *buf, int size)
{/*{{{*/
    char fullexe[1024];
    char tmp[1024];
    char *ptr;
    ptr = (char *)malloc((size_t)size);
    if(NULL != ptr){
        memset(ptr,0,size);
        snprintf(ptr, size, "/proc/%d/exe",getpid());
    } else {
        return -1;
    }
    readlink(ptr,fullexe,size);

	snprintf(fullexe,size,"/Users/mawentao/mawt/du/output/du/xxx");

    split_full_path_file(fullexe, buf, size, tmp, 1024);
    return 0;
}/*}}}*/


/**
 * 初始化话du数据
 **/
int init_du(du_t &du, const char *conf_path, const char *conf_file)
{/*{{{*/
    Configure cfg;
    cfg.load(conf_path, conf_file);
    const char *author = cfg.getstr("author").c_str();
    snprintf(du._AUTHOR_, MAX_FILENAME_LEN, "%s", author);
    get_exe_path(du.exe_path, MAX_FILENAME_LEN);
    getcwd(du.cur_path, MAX_FILENAME_LEN);
    snprintf(du.temp_path, MAX_FILENAME_LEN, "%s/template", du.exe_path);
    snprintf(du._WORK_ROOT_, MAX_FILENAME_LEN, ".");

    //1. set _YEAR_
    struct tm *strt;
    time_t t;
    time(&t);
    strt = localtime(&t);
    strftime(du._YEAR_, 16, "%Y", strt);

    //2. set _DATETIME_
    strftime(du._DATETIME_, 32, "%Y-%m-%d %H:%M:%S", strt);

    //3. set _FILENAME_ 默认是当前路径的文件夹名
    char tmp[1024];
    split_full_path_file(du.cur_path, tmp, 1024, du._FILENAME_, MAX_FILENAME_LEN);

    return 0;
}/*}}}*/

/**
 * 删除文件后缀名
 */
char* trip_suffix(char *str)
{/*{{{*/
    char *ps = str;
    char *pc = str;
    while(*ps){
        if(*ps == '.'){
            //*ps = '\0';
            pc = ps;
            break;
        }
        ++ps;
    }
    if (pc!=str) *pc='\0';
    return str;
}/*}}}*/

void get_parent(char *path)
{/*{{{*/
    char *p = path;
    char *t = path;
    while(*p){
        if(*p == '/'){
            t = p;
        }
        ++p;
    }
    *t = '\0';
}/*}}}*/

bool is_file(const char *fname)
{/*{{{*/
    struct stat file_info;
    stat(fname, &file_info);
    mode_t mode = file_info.st_mode;
    return S_ISREG(mode);
}/*}}}*/

bool is_dir(const char *fname)
{/*{{{*/
    struct stat file_info;
    stat(fname, &file_info);
    mode_t mode = file_info.st_mode;
    return S_ISDIR(mode);
}/*}}}*/

char* lower(char *str)
{/*{{{*/
    char *ps = str;
    while(*ps){
        if(*ps >= 65 && *ps <= 90){
            *ps += 32;
        }
        ++ps;
    }
    return str;
}/*}}}*/

char* upper(char *str)
{/*{{{*/
    char *ps = str;
    while(*ps){
        if(*ps >= 97 && *ps <= 122){
            *ps -= 32;
        }
        ++ps;
    }
    return str;
}/*}}}*/

/** 根据文件名生成类名 */
void generate_class_name(const char *filename, char *classname)
{/*{{{*/
    const char *pf = filename;
    char *pc = classname;
    bool is_start = true; 
    while(*pf){
        if(*pf == '_'){
            is_start = true; 
            ++pf;   
            continue;
        }       
        *pc = *pf;
        if(is_start && *pc >= 97 && *pc <= 122){
            *pc -= 32;
        }       
        is_start = false;
        ++pc;   
        ++pf;   
    }
    *pc = '\0'; 
}/*}}}*/

void exe_sed(const char *src, const char *dest, const char *sed)
{/*{{{*/
    char cmd[10240];
    snprintf(cmd, 10240, "sed '%s' %s > %s", sed, src, dest);
    system(cmd);
    snprintf(cmd, 10240, "generate file: [ %s ].", dest);
    print_exe(cmd);
}/*}}}*/

/** 判断是否是work_root */
bool is_work_root(const char *path)
{/*{{{*/
    std::string d(path);
    d += "/public";
    if(access(d.c_str(), 0) != 0){
        return false;
    }
    d = path;
    d += "/lib2-64";
    return access(d.c_str(), 0) == 0;
}/*}}}*/

/** 检查是否在svn_root下 */
bool check_work_root(char *path, char *WORK_ROOT)
{/*{{{*/
    strcpy(WORK_ROOT, ".");
    do{
        if(is_work_root(path)){
            return true;
        }
        get_parent(path);
        strcat(WORK_ROOT, "\\/..");
    }while(*path);
    return false;
}/*}}}*/

/** 生成文件 */
void gen_file(const char *tempfile, const char *destfile, du_t &du, bool relace)
{
    //1. 判断目标文件是否已存在
    char buf[1024];
    if (is_file(destfile) && !relace) {
        snprintf(buf, 1024, "file already exists: %s [ignore]", destfile);
        print_exe(buf);
        return;
    }
    //2. sed
    std::string sed;
    sed = sed+"s/_YEAR_/"+du._YEAR_+"/g;";
    sed = sed+"s/_DATETIME_/"+du._DATETIME_+"/g;";
    sed = sed+"s/_AUTHOR_/"+du._AUTHOR_+"/g;";
    sed = sed+"s/_HEADFILENAME_/"+du._HEAD_FILENAME_+"/g;";
    sed = sed+"s/_FILENAME_/"+du._FILENAME_+"/g;";
    sed = sed+"s/_PROJECTNAME_/"+du._FILENAME_+"/g;";
    sed = sed+"s/_WORKROOT_/"+du._WORK_ROOT_+"/g;";
    sed = sed+"s/_NAMESPACE_/"+du._FILENAME_+"/g;";
    sed = sed+"s/_UPNAMESPACE_/"+du._HEAD_FILENAME_+"/g;";
    //3. generate file
    exe_sed(tempfile, destfile, sed.c_str());
    snprintf(buf, 1024, "chmod +x %s", destfile);
    exe_cmd(buf);
}

/* 解析命令行参数，读取配置并初始化 */
int parse_args(int argc, char *argv[], du_t &du)
{
    //0. 获取默认值
    char exepath[1024];
    char confpath[1024];
    get_exe_path(exepath, 1024);
    snprintf(confpath,1024, "%s/conf/", exepath);
    init_du(du, confpath);
    //1. 解析命令行参数
    int opt;
    struct option longopts[] = {
        {"help", 0, NULL, 'h'},
        {"version", 0, NULL, 'v'}
    };
    while((opt=getopt_long(argc, argv, "hv", longopts, NULL)) != -1){
        switch(opt) {
        case 'h':
            print_usage(argv[0]);
            exit(0);
        case 'v':
            printf("%s %s\n", argv[0], VERSION);
            exit(0);
        }
    }
    if(argc > 1){
        snprintf(du._FILENAME_, MAX_FILENAME_LEN, "%s", trip_suffix(argv[1]));
    }
    snprintf(du._HEAD_FILENAME_, MAX_FILENAME_LEN, "%s", du._FILENAME_);
    upper(du._HEAD_FILENAME_);
    generate_class_name(du._FILENAME_, du._CLASSNAME_);
    return 0;
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
