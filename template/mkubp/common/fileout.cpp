/***************************************************************************
 *
 * Copyright (c) 2012 Baidu.com, Inc. All Rights Reserved
 * $Id: fileout.cpp, v1.0.0, 2012-09-15 09:14:36, mawentao Exp $
 *
 **************************************************************************/

/**
 * @file   fileout.cpp
 * @author mawt(mawentao@baidu.com)
 * @date   2012-09-15 09:14:36
 * @brief  fileout.cpp
 **/

#include "fileout.h"
#include "ub_log.h"

/**
 * @brief ��ʼ���ļ������
 * @param fname_prefix : const char * �ļ���ǰ׺
 * @param bufcount : uint32_t �����������С
 * @param bufsize : uint32_t ������¼�Ĵ�С
 * @param write_interval_s : uint32_t ����д�ļ������ʱ����
 * @return int 0-�ɹ� -1:ʧ��
 */
int FileOut::init(const char *fname_prefix, uint32_t bufcount, uint32_t bufsize, uint32_t write_interval_s)
{/*{{{*/
    //1. copy args
    strncpy(m_fo.file_prefix, fname_prefix, 1024);
    m_fo.bufcount = bufcount;
    m_fo.bufsize = bufsize;
    m_fo.write_interval_s = write_interval_s;
    m_fo.buf = new char*[bufcount];
    if(!m_fo.buf){
        return -1;
    }
    m_fo.size = 0;
    //2. create buf
    for(uint32_t i=0; i<bufcount; ++i){
        m_fo.buf[i] = new char[bufsize];
        if(!m_fo.buf[i]){
            return -1;
        }
        m_fo.buf[i][0] = '\0';
    }
    //3. init buflock
    int ret = pthread_mutex_init(&m_fo.buflock, NULL);
    if(ret != 0){
        return -1;
    }
    return 0;
}/*}}}*/

/** @brief ���������ͷ���Դ */
FileOut::~FileOut()
{/*{{{*/
    //1. release buf
    for(uint32_t i=0; i<m_fo.bufcount; ++i){
        delete[] m_fo.buf[i];
        m_fo.buf[i] = NULL;
    }
    delete[] m_fo.buf;
    m_fo.buf = NULL;
    //2. destroy buflock
    pthread_mutex_destroy(&m_fo.buflock);
}/*}}}*/

/** @brief д������ */
void FileOut::writebuf(const char *msg)
{/*{{{*/
    if(msg == NULL){
        return;
    }
    //�����ٽ���
    pthread_mutex_lock(&m_fo.buflock);
    //1. ���������������д���ļ�
    if(m_fo.size == m_fo.bufcount){
        append_to_file(m_fo);
    }
    //2. д�뻺����
    strncpy(m_fo.buf[m_fo.size++], msg, m_fo.bufsize);
    //�˳��ٽ���
    pthread_mutex_unlock(&m_fo.buflock);
}/*}}}*/

/** ��ȡ��ǰʱ����ַ��� */
void now_str_time(char *tbuf, size_t size, const char *format)
{/*{{{*/
    tbuf[0] = '\0';
    struct tm *strt = NULL;
    time_t t;
    time(&t);
    strt = localtime(&t);
    if(!strt){
        return;
    }
    strftime(tbuf, size-1, format, strt);
}/*}}}*/

/** @brief ��buf�е�����д���ļ��� */
void FileOut::append_to_file(file_out_t &fo)
{/*{{{*/
    //1. �����ļ���
    char fname[1024];
    char timestr[128];
    now_str_time(timestr, 64, "%Y%m%d");
    snprintf(fname, 1024, "%s%s", fo.file_prefix, timestr);
//    printf("%s\n", fname);
    UB_LOG_TRACE("FileOut: write to file. [file:%s] [size:%u]", fo.file_prefix, fo.size);
    //2. ����ӷ�ʽ���ļ�
    FILE *fp = fopen(fname, "a");
    if(!fp){
        UB_LOG_WARNING("open file fail. [fname:%s]", fname);
        return;
    }
    //3. ���������е�����д���ļ�
    for(uint32_t i=0; i<fo.size; ++i){
        now_str_time(timestr, 64, "%H:%M:%S");
        fprintf(fp, "%s\t%s\n", timestr, fo.buf[i]);
    }
    //4. ��ջ�����
    fo.size = 0;
    //5. �ر��ļ�
    fclose(fp);
}/*}}}*/

/** ��ʱд�̻߳ص����� */
void * FileOut::m_callback_fun(void *args)
{
    file_out_t *fm = (file_out_t*)args;
    ub_log_initthread(fm->file_prefix); //���߳���־
    while(true){
        sleep(fm->write_interval_s);
        //�����ٽ���
        pthread_mutex_lock(&fm->buflock);
        //������������գ�д���ļ�
        if(fm->size > 0){
            append_to_file(*fm);
        }
        //�˳��ٽ���
        pthread_mutex_unlock(&fm->buflock);
    }
    ub_log_closethread();     //�ر��߳���־
}

// vim:fdm=marker:nu:ts=4:sw=4:expandtab
