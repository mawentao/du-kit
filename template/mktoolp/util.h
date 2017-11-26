/***************************************************************************
 *
 * Copyright (c) _YEAR_ Baidu.com, Inc. All Rights Reserved
 * $Id: util.h, v1.0.0, _DATETIME_, _AUTHOR_ Exp $
 *
 **************************************************************************/

/**
 * @file   util.h
 * @author _AUTHOR_
 * @date   _DATETIME_
 * @brief  util.h
 **/

#ifndef UTIL_H
#define UTIL_H


#include <iostream>
#include <string.h>


/** ������ */
class Util{
public:
	/**
	 * @brief ��ӡ����
	 * @param[in] cur: uint32_t ��ǰ������к�
     * @param[in] sum: uint32_t ������
     * @param[in/out] last: �ϴδ�ӡ�Ľ���
     * @return void
     **/
	static void print_progress(uint32_t cur, uint32_t sum, uint32_t &last);

	/**
     * @brief ȥ��β���հ׷�(\n\r\t )
	 * @param[in/out] str: char *
	 * @return void
     **/
	static void str_rtrim(char *str);

	/**
     * @brief ͳ���ļ�����
	 * @param[in] fname: const char *
	 * @return uint64_t
     **/
	static uint64_t stat_line_num(const char *fname);

    /**
     * @brief ����ȫ·���ļ�����·�����ļ���
     * @param[in] fullpath: const char *
     * @param[in/out] path: char *
     * @param[in/out] path_size: uint32_t
     * @param[in/out] file: char *
     * @param[in/out] file_size: uint32_t
	 * @return int 0-�ɹ� ����-ʧ��
     **/
	static int split_full_path_file(const char *fullpath, char *path, uint32_t path_size, char * file, uint32_t file_size);

	/**
     * @brief ��ȡ��ִ�г����·��
     * @param[in/out] buf: char *
     * @param[in] size: uint32_t
     * @return int: 0-�ɹ� ����-ʧ��
     */
	static int get_exe_path(char *buf, uint32_t size);


    ~Util(){}

private:
	Util(){}

    Util(const Util&);
    Util& operator=(const Util&);
};


#endif
