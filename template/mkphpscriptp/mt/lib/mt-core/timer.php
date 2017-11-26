<?php

/***********************************************************
 * @file:   Timer.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2014-10-05 10:25:39
 * @modify: 2014-10-05 10:25:39
 * @brief:  计时器模块
 ***********************************************************/


class Timer
{
	private static $_timemap = array();

	/* 获取微秒数 */
	private static function get_micro_time()
	{
		list($usec, $sec) = explode(" ",microtime());
		return ((float)$usec + (float)$sec);
	}

	/* 记录起始时间 */
	public static function begin($taskkey)
	{
		$nt = self::get_micro_time();
		if (!isset(self::$_timemap[$taskkey])) {
			self::$_timemap[$taskkey] = array (
				"stime" => $nt,
				"etime" => $nt,
			);
		} else {
			self::$_timemap[$taskkey]["stime"] = $nt;
		}
	}

	/* 记录结束时间 */
	public static function end($taskkey)
   	{
		$nt = self::get_micro_time();
		if (!isset(self::$_timemap[$taskkey])) {
			self::$_timemap[$taskkey] = array (
				"stime" => $nt,
				"etime" => $nt,
			);
		} else {
			self::$_timemap[$taskkey]["etime"] = $nt;
		}

		if (Env::isDebug()) {
			$timediff = self::get($taskkey);
			Env::getlog("timer")->notice("$taskkey [$timediff]");
		}
	}

	/* 获取耗时 */
	public static function get($taskkey)
	{
		if (!isset(self::$_timemap[$taskkey])) {
			return 0;
		}

		$stime = self::$_timemap[$taskkey]["stime"];
		$etime = self::$_timemap[$taskkey]["etime"];
		$timediff = ($etime-$stime)*1000;
		if ($timediff < 1000) {
			$timediff = number_format($timediff, 3). " ms";
		} else {
			$timediff /= 1000;
			$timediff = number_format($timediff, 3). " s";
		}
		return $timediff;
	}
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
