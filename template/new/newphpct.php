#!/usr/bin/env php
<?php
/***********************************************************
 * @file:   _FILENAME_.php
 * @author: _AUTHOR_
 * @create: _DATETIME_
 * @modify: _DATETIME_   
 * @brief:  _FILENAME_.php
 ***********************************************************/

/////////////////////////////////////////////////////////////

/* 当前脚本的日志文件名：当前脚本文件全路径名.log */
define("LOG_FILE", __FILE__.".log");
$log_fp = fopen(LOG_FILE, "a");

/** 记录成功日志 */
function WRITE_SUCC_LOG($msg)
{/*{{{*/
	global $log_fp;
	if($log_fp){
		fprintf($log_fp, "SUCC: [%s] * %s\n", date('m-d H:i:s'), $msg);
	}
}/*}}}*/

/** 记录失败日志 */
function WRITE_FAIL_LOG($msg)
{/*{{{*/
	global $log_fp;
	if($log_fp){
		fprintf($log_fp, "FAIL: [%s] * %s\n", date('m-d H:i:s'), $msg);
	}
}/*}}}*/

/** 关闭日志文件 */
function CLOSE_LOG()
{/*{{{*/
	global $log_fp;
	if($log_fp){ fclose($log_fp); }
}/*}}}*/

/** 获取微秒时间 */
function getmicrotime()
{/*{{{*/
    list($usec, $sec) = explode(" ",microtime());
    return ((float)$usec + (float)$sec);
}/*}}}*/

/////////////////////////////////////////////////////////////


/* 下载文件存放的位置 */
define("DATA_PATH", "/home/mawt/tmp/data");
/* 下载文件名 */
define("FILE_PREFIX", "wordlist.");


if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME']))
{
	/* 下载昨天的文件 */
	$date = date("Ymd", time()-86400);
	/* 下载的文件名 */
	$file = DATA_PATH."/".FILE_PREFIX.$date;

	// 文件不存在时，执行
	if(!is_file($file))
	{
		$start = getmicrotime();  //!< 记录起始时间

		//TODO: 在此处添加获取或处理文件的脚本
    	system("touch $file");
		usleep(1000000);


		$end = getmicrotime();    //!< 记录结束时间
		$proctime = number_format(($end-$start), 4);
		WRITE_SUCC_LOG("Done. [file:$file] [proctime: $proctime s].");
	}
	// 如果文件已经存在，则退出
	else{WRITE_FAIL_LOG("file is exist. [file: $file].");}

	CLOSE_LOG();  //关闭本脚本日志文件
}

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
