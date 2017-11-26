<?php
/**
 * system 命令工具类
 **/
class dbdump
{
	private static $dblist;
    private static $dbcfgmap;
    
	public static function init($conf)
	{
		self::$dblist = array();
        self::$dbcfgmap = array();
        // 所有lang开头的配置
        $reg = "/^lang/i";
		foreach ($conf as $key => $dbconf) {
			if (preg_match($reg, $key)) {
				$db = $dbconf['db'];
				self::$dblist[$db] = "mysql -h".$dbconf['host'].
			        " -P".$dbconf['port'].
			        " -u".$dbconf['user'].
			        " -p".$dbconf['pass'].
					" --default-character-set=utf8";
                self::$dbcfgmap[$db] = $dbconf;
			}
		}
	}
/*
    // 根据mysql表创建hive表
    public static function create_hive_table_from_mysql($mysql_db,$mysql_tabel,$hive_db,$hive_table)
    {
        if (!isset(self::$dbcfgmap[$mysql_db])) {
            ublog::fatal("unknow database $mysql_db");
            return;
        }
        $mysqldbconf = self::$dbcfgmap[$mysql_db];      
        $host = $mysqldbconf['host'];
        $port = $mysqldbconf['port'];
        $user = $mysqldbconf['user'];
        $pass = $mysqldbconf['pass'];
        $cmd = "sqoop create-hive-table --connect jdbc:mysql://$host:$port/$mysql_db --username $user --password $pass ".
               "--table $mysql_tabel --hive-table $hive_db.$hive_table --fields-terminated-by \"\\t\" --lines-terminated-by \"\\n\" ".
               "--hive-partition-key dt";
        die($cmd);
    }
*/


    // 执行sql导出到文件
    public static function hive_export_sql_to_file($sql,$file)
    {
        $sql = "set mapreduce.job.queuename=root.hailangshiyebu-jishubu.default;$sql";
        $cmd = "hive -e \"$sql\" > $file";
        runcmd($cmd);
    }   

    // 上传文件到hive表
    public static function hive_import_file_to_table($db,$table,$partion,$file)
    {
        // hadoop路径
        $hdfs_dir = "/user/lang_bi/warehouse/$db.db/$table/$partion";
        //1. 删除原文件
        $cmd = "hadoop fs -rm $hdfs_dir/$table";
        runcmd($cmd);
        $cmd = "hadoop fs -rm $hdfs_dir/_SUCCESS";
        runcmd($cmd);
        //2. 上传文件
        $sql = "use $db;load data local inpath '$file' into table $db.$table PARTITION($partion);";
        $cmd = "hive -e \"$sql\"";
        runcmd($cmd);
        //3. 创建_SUCCESS文件
        $cmd = "hadoop fs -touchz $hdfs_dir/_SUCCESS";
        runcmd($cmd);
    }

    // 执行sql导出到文件
    public static function mysql_export_sql_to_file($db,$sql,$file)
    {
        $cmd = self::$dblist[$db]." -N -e \"use $db;$sql\" > $file";
        runcmd($cmd);
    }

    // 将文件导入db中的table表
    public static function mysql_load_data_infile($file, $db, $table, $replace=false) 
    {
        if (!isset(self::$dblist[$db])) {
            ublog::warning("db_not_config: $db");
            return;
        }
        $rp = $replace ? 'replace' : 'ignore';
        $sql = "use $db; load data local infile '$file' $rp into table $table";
        $cmd = self::$dblist[$db]." -N -e \"$sql\"";
        runcmd($cmd);
    }

    // 获取mysql表的建表语句
    public static function get_mysql_create_table_sql($db,$table)
    {/*{{{*/
        // 导出建表语句到文件
        // 数据表的字段可能会改变，需要把建表语句保存下来
        $tmpfile = fileutil::genFileName("$db.$table.create.sql");
        die($tmpfile);
        if (!dis_file($tmpfile)) {
            $dbname = self::get_dbname($db);
            $sql = "use $dbname; show create table $table \G";
            $cmd = self::get_mysql_bin($db)." -N -e \"$sql\" > $tmpfile";
            runcmd($cmd);
        }   
        $res = file_get_contents($tmpfile);
        // 处理建表语句,发现不同mysql版本支持的建表语句不同,这里做下适配
        $pos = strpos($res,'CREATE TABLE');
        $res = substr($res, $pos);
        $res = str_replace('CREATE TABLE','CREATE TABLE IF NOT EXISTS', $res);
        $res = str_replace('ON UPDATE CURRENT_TIMESTAMP','', $res);
        $res = str_replace('DEFAULT CURRENT_TIMESTAMP','', $res);
        $res = str_replace('COLLATE utf8_bin','', $res);  //!< 去除二进制显示
        $res = str_replace('COLLATE=utf8_bin','', $res);  //!< 去除二进制显示
        $res = str_replace("\n",'', $res);
        $res = str_replace("\r",'', $res);
        return $res;
    }/*}}}*/

}
// vim600: sw=4 ts=4 fdm=marker syn=php
?>
