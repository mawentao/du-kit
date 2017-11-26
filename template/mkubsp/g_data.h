/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: g_data.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   g_data.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  g_data.h
 **/

#ifndef G_DATA_H
#define G_DATA_H

#include <iostream>
#include <Configure.h>
#include "ubserver_frame.h"


/** 函数返回码 */
const int FUN_SUCC = 0;
const int FUN_FAIL = -1;

/** 文件名|路径名最大长度 */
const int FILE_NAME_LEN = 1024;


/** 全局配置数据结构 */
struct conf_t{
    ub_svr_t svr_conf;          //!< UBServer配置

    uint32_t read_buf_size;     //!< 读取缓冲区大小
    uint32_t write_buf_size;    //!< 写入缓冲区大小
    uint32_t tmp_buf_size;      //!< 临时缓冲区大小

public:
    /* 调试用 */
    void output(FILE *fp=stdout)
	{
		fprintf(fp, "g_conf:{");
		fprintf(fp, " [read_buf_size:%u]",  read_buf_size);
		fprintf(fp, " [write_buf_size:%u]", write_buf_size);
		fprintf(fp, " [tmp_buf_size:%u]",   tmp_buf_size);
		fprintf(fp, " }\n");
	}
};

#endif
