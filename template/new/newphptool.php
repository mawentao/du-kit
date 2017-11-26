#!/usr/bin/env php
<?php
/***********************************************************
 * @file:   _FILENAME_.php
 * @author: _AUTHOR_
 * @create: _DATETIME_
 * @modify: _DATETIME_   
 * @brief:  _FILENAME_.php
 ***********************************************************/

/** 全局配置 */
$g_conf = array(
	"input_file"=>"",       //!< 输入文件
	"output_file"=>"",	    //!< 输出文件
	"line_num"=>0			//!< 处理的行数
);

/* 统计文件一共多少行 */
function get_file_line_num($file)
{/*{{{*/
	if(!is_file($file)){
		fprintf(STDERR, "get_file_line_num fail: can not open $file.\n");
        exit(-1);
	}
    $fp=fopen($file,'r');
    if(!$fp){
		fprintf(STDERR, "get_file_line_num fail: can not open $file.\n");
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

/* 打印进度条 */
function print_progress($cur, $sum, &$last) 
{/*{{{*/
    //1. 计算当前进度
    $step = 0;
    if($sum < 100){ 
        $step = $cur * (100/$sum);
    }else{  
        $step = ($cur*100) / $sum; 
    }
    //2. 如果当前进度已更新，则显示
    if($step>=$last && $step <= 100){
        printf("progress: [%-100s] %d%%\r", str_repeat('#',$step), $step); 
        ++$last;
    }
}/*}}}*/

/** 获取微秒数，用于统计时间 */
function getmicrotime()
{/*{{{*/ 
    list($usec, $sec) = explode(" ",microtime()); 
    return ((float)$usec + (float)$sec); 
}/*}}}*/

/** 打印使用帮助 */
function print_usage($pname)
{/*{{{*/
	printf("[usage]: $pname input_file [-n line_num] [-o output_file]\n");
	exit(0);
}/*}}}*/

/** 解析命令行参数 */
function parse_args(&$argc, &$argv)
{/*{{{*/
	global $g_conf;

	if($argc < 2){
		print_usage($argv[0]);
	}

	$state = 0;
	for($i=1; $i<$argc; ++$i){
		if($argv[$i] == '-n'){
			$state = 1;
			continue;
		}
		if($argv[$i] == '-o'){
			$state = 2;
			continue;
		}
		if($argv[$i] == '-h'){
			print_usage($argv[0]);
		}
		switch($state){
			case 0: $g_conf["input_file"] = $argv[$i]; break;
			case 1: $g_conf["line_num"] = intval($argv[$i]); break;
			case 2: $g_conf["output_file"] = $argv[$i]; break;
		}
	}

	if($g_conf["input_file"] == ""){print_usage($argv[0]);}
	if($g_conf["output_file"]== ""){
		$g_conf["output_file"] = $g_conf["input_file"].".out";
	}
	if($g_conf["line_num"] == 0){
		$g_conf["line_num"] = get_file_line_num($g_conf["input_file"]);
	}
}/*}}}*/


/** 程序开始 */
if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME'])) 
{
	//1. 解析命令行参数
	parse_args($argc, $argv);
	//var_dump($g_conf);

	//2. 打开输入输出文件
	if(!is_file($g_conf["input_file"])){
		fprintf(STDERR, $g_conf["input_file"]." is not a file.\n");
		exit(-1);
	}
	$fi = fopen($g_conf["input_file"], "r");
	if(!$fi){
		fprintf(STDERR, "can not open file: ".$g_conf["input_file"].".\n");
		exit(-1);
	}
	$fo = fopen($g_conf["output_file"], "w");
	if(!$fo){
		fprintf(STDERR, "can not open file: ".$g_conf["output_file"].".\n");
		exit(-1);
	}
	
	//3. 处理
    $cur_line = 0;
    $last_line = 0;
	$line_num = $g_conf["line_num"];

	$START_TIME = getmicrotime();
    while(!feof($fi))
    {
		$line = trim(fgets($fi));
        if($line == ""){continue;}

		///////////////////////////////////////
		//TODO: 处理行

		$arr = explode("\t", $line);

		fprintf($fo, "%s\n", $arr[0]);

		///////////////////////////////////////

		//打印进度条
		++$cur_line;
        if($line_num>0){
            print_progress($cur_line, $line_num, $last_line);
        }
		if($cur_line >= $line_num){break;}
	}
	$END_TIME = getmicrotime();


	//99. 关闭退出
	fclose($fi);
	fclose($fo);

	$del = $END_TIME-$START_TIME;
	printf("\n\nDone. [time: ".$del."s] [output_file: ".$g_conf["output_file"]."]\n");
}

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
