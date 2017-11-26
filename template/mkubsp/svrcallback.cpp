/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: svrcallback.cpp, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   svrcallback.cpp
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  svrcallback.cpp
 **/

#include "svrcallback.h"

#define MAX_TRESPASSING_FILE_LEN 2048


/** for debug */
static void print_head(const nshead_t *head)
{/*{{{*/
    printf("nshead: {\n");
    printf("\tid : %d\n", head->id);
    printf("\tversion : %d\n", head->version);
    printf("\tlog_id : %d\n", head->log_id);
    printf("\tprovider : %s\n", head->provider);
    printf("\tmagic_num : %d\n", head->magic_num);
    printf("\treserved : %d\n", head->reserved);
    printf("\tbody_len : %d\n", head->body_len);
    printf("}\n");
}/*}}}*/

/** for debuf */
static void print_body(const mc_pack_t *pack)
{/*{{{*/
    uint32_t tmp_size = mc_pack_get_size(pack);
    char *tmp_buf = new char[tmp_size];
    if(tmp_buf){
        int ret = mc_pack_pack2json(pack, tmp_buf, tmp_size);
        if(ret == 0){
            printf("mc-pack : %s\n", tmp_buf);
        }
        delete [] tmp_buf;
    }
}/*}}}*/

/**
 * @brief server_t 回调函数
 * @param &req_nshead       请求 nshead
 * @param req        请求包体
 * @param res        应答包体
 * @return FUN_SUCC / FAIL
 */
int svr_callback(int sock, nshead_t *head, mc_pack_t *req_pack, char *tmp_buf, size_t tmp_size)
{
    //1. 获取本次请求处理过程独占的内存
    uint64_t tid = pthread_self();
    uint32_t nshead_size = sizeof(nshead_t);
    int ret = 0;

    char *read_buf  = (char*)ub_server_get_read_buf();
    char *write_buf = (char*)ub_server_get_write_buf();
    uint32_t read_size = ub_server_get_read_size();
    uint32_t write_size = ub_server_get_write_size();
    uint32_t write_timeout = ub_server_get_write_timeout();
///*
    //调试用
    printf("\nreceive a pack, pack_size: %u [tid:%lu]\n", mc_pack_get_size(req_pack), tid);
    print_head(head);
    print_body(req_pack);
// */

    //2. 封装返回包（write_buf: head+body，框架会自动把write_buf中的mcpack返回）
    //请求包原封不动返回
    memcpy(write_buf, read_buf, mc_pack_get_size(req_pack)+nshead_size);

/*
    nshead_t *res_head = (nshead_t *)write_buf;
    *res_head = *head;
    mc_pack_t *res_pack;
    res_pack = mc_pack_open_rw(write_buf+nshead_size, write_size-nshead_size, tmp_buf, tmp_size);
    //res_pack = mc_pack_open_w(2, write_buf+nshead_size, write_size-nshead_size, tmp_buf, tmp_size);
    mc_pack_put_uint32(res_pack, "status", 0);
    res_head->body_len = mc_pack_get_size(res_pack);
    print_body(res_pack);  //!< 调试用
*/

FUNC_OUT:

    return FUN_SUCC;
}/*}}}*/



// vim:fdm=marker:nu:ts=4:sw=4:expandtab
