<?php
/**
 * 文件工具函数
 **/
class fileutil
{
    private static $_data_root;
    private static $_file_dir;
	private static $_dm_data_root;
	private static $_dm_file_dir;

	// 文件系统初始化
	public static function init($dataroot)
	{
		self::$_data_root = rtrim($dataroot,'/');
		$stat_time = timeutil::getStatTime();
		self::$_file_dir = self::$_data_root."/".date("Y",$stat_time)."/".date("m",$stat_time)."/".date("d",$stat_time);
		if (!is_dir(self::$_file_dir)) {
			mkdir(self::$_file_dir, 0777, true);
		}
	}

	// 获取文件存储路径
	public static function getFileDir($isdm=false) {
		if ($isdm) return self::$_dm_file_dir;
		return self::$_file_dir;
	}

	// 生成ODS文件名
	public static function genOdsFileName($table) {
		return self::genFileName("$table.ods");
	}

	// 生成文件名
	public static function genFileName($fname) {
		return self::$_file_dir."/".$fname;
	}

}
// vim600: sw=4 ts=4 fdm=marker syn=php
?>
