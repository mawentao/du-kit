<?php

/* ǿ��ת��UTF-8���� */
function toUTF8($str)
{/*{{{*/
	if (preg_match('/^.*$/u', $str) <= 0) {
		return iconv("GBK","UTF-8//ignore", $str);
	} else {
		return $str;
	}
}/*}}}*/

/* ǿ��ת��GBK���� */
function toGBK($str)
{/*{{{*/
	if (preg_match('/^.*$/u', $str) <= 0) {
		return $str;
	} else {
		return iconv("UTF-8","GBK//ignore", $str);
	}
}/*}}}*/

/* ��ӡ���������ű�Ӧ�ã� */
function print_progress($cur, $sum, &$last) 
{/*{{{*/
	//1. ���㵱ǰ����
	$step = 0;
	if($sum < 100){ 
		$step = $cur * (100/$sum);
	}else{  
		$step = ($cur*100) / $sum; 
	}       
	//2. �����ǰ�����Ѹ��£�����ʾ
	if($step>=$last && $step <= 100){
		printf("progress: [%-100s] %d%%\r", str_repeat('#',$step), $step); 
		++$last;
	}       
}/*}}}*/

/* ��ȡ΢����������ͳ��ʱ�� */
function get_micro_time()
{/*{{{*/ 
	list($usec, $sec) = explode(" ",microtime());
	return ((float)$usec + (float)$sec);
}/*}}}*/

/* ͳ���ļ�һ�������� */
function count_line_num($file)
{/*{{{*/
	if(!is_file($file)){
		fprintf(STDERR, "UF.count_line_num fail: can not open $file.\n");
		exit(-1);
	}
	$fp=fopen($file,'r');
	if(!$fp){
		fprintf(STDERR, "UF.count_line_num fail: can not open $file.\n");
		exit(-1);
	}
	$res = 0; 
	while(!feof($fp)){
		fgets($fp);
		++$res;
	}   
	fclose($fp);
	return $res-1;
}/*}}}*/

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
