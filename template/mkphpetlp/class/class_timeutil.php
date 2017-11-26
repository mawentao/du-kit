<?php
/**
 * 时间工具函数
 **/
class timeutil
{
	private static $_stat_day;        //!< 统计日期
	private static $_stat_time;       //!< 统计时间戳
	private static $_stat_time_begin; //!< 统计日期开始时间戳
    private static $_stat_time_end;   //!< 统计日期结束时间戳

	// 时间窗口初始化
	public static function init($arg)
	{
		$ntm = time();
		$stattm = $ntm - 86400;   //!< 默认统计日期是当前时间前一天
		$tm = strtotime($arg);	
		if ($tm<$ntm && $tm > 1462032000) {
			$stattm = $tm;
		}
		self::$_stat_time = $stattm;
		self::$_stat_day = date('Ymd', $stattm);
		
		$day_begin = date('Y-m-d 00:00:00', $stattm);
		self::$_stat_time_begin = strtotime($day_begin);
		
		$day_end = date('Y-m-d 23:59:59', $stattm);
		self::$_stat_time_end = strtotime($day_end);
	}
	
	// 获取统计日期
    public static function getStatDay()
	{
		return self::$_stat_day;	
	}

	// 获取统计时间戳
	public static function getStatTime()
	{
		return self::$_stat_time;
	}

	// 获取统计日期开始时间戳
	public static function getStatTimeBegin()
	{
		return self::$_stat_time_begin;
	}

	// 获取统计日期结束时间戳
	public static function getStatTimeEnd()
	{
		return self::$_stat_time_end;
	}
}
// vim600: sw=4 ts=4 fdm=marker syn=php
?>
