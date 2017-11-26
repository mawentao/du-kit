#!/usr/bin/env php
<?php
/***********************************************************
 * @file:   main.php
 * @author: _AUTHOR_
 * @create: _DATETIME_
 * @modify: _DATETIME_
 * @brief:  main.php
 ***********************************************************/

ini_set('date.timezone','Asia/Shanghai');

define("APP_PATH",    realpath(dirname(__FILE__)));
define("MT_PATH",     APP_PATH."/mt");
define("CONF_PATH",   APP_PATH."/conf");
define("MODEL_PATH",  APP_PATH."/models");
define("SCRIPT_PATH", APP_PATH."/scripts");

require_once(MT_PATH."/lib/env.php");

Env::appendLoaderPath(MODEL_PATH);
Env::appendLoaderPath(SCRIPT_PATH);

$conf = Env::getconf("main");
Env::init($conf["log"]);

//Env::setpdo($conf["mysql"]);

// 设置Debug模式
if (!$conf["debug"]) {
	ini_set('display_errors', 'off');
	Env::setDebug(false);
} else {
	Env::setDebug(true);
}
error_reporting(E_ALL);
///////////////////////////////////////////////////


// 所有脚本基类
class BaseScript
{
	protected $_res;  //!< 脚本执行的返回码

	// 模板方法
	final public function run($args)
	{
		$this->beforeRun();
		$this->_res = $this->execute($args);
		$this->afterRun();
	}

	// 执行脚本（子类重写此方法）
	protected function execute($args)
	{
	}

	//切面：script运行前调用
	protected function beforeRun()
	{
		//1. 记录开始时间 && 打日志
		Timer::begin("SCRIPT");
		$log = "start  [tm:".time()."]";
		LOG_TRACE($log);
	}

	//切面：script运行后调用
	protected function afterRun()
	{
		//1. 记录结束时间 && 计算运行耗时
		Timer::end("SCRIPT");
		$timespan = Timer::get("SCRIPT");

		//2. 记录运行日志
		$log = "finish [tm:".time()."] [timespan:$timespan] [res:".$this->_res."]";
		LOG_TRACE($log);
	}
}

/////////////////////////////////////////////
// 打日志函数
function pack_log($log) {
	return "[".Env::get("SCRIPT_NAME")."]\t[pid:".getmypid()."]\t".$log;
}
function LOG_DEBUG($log)  { Env::getlog()->debug(pack_log($log)); }
function LOG_TRACE($log)  { Env::getlog()->trace(pack_log($log)); }
function LOG_NOTICE($log) { Env::getlog()->notice(pack_log($log)); }
function LOG_WARNING($log) { Env::getlog()->warning(pack_log($log)); }
function LOG_FATAL($log) { Env::getlog()->fatal(pack_log($log)); }
//////////////////////////////////////////////


if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME']))
{
	if (!isset($argv[1]) || $argv[1]=="") {
		$exe = $argv[0];
		echo "[usage]: php $exe SCRIPT_NAME\n";
		exit(0);
	}
	Env::set("SCRIPT_NAME", $argv[1]);
	$script = new $argv[1];
    $args = array_slice($argv, 2);
	$script->run($args);
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
