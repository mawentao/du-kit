<?php
/**
 * system 命令工具类
 **/
class mysql_util
{
    // 获取mysql_bin
    private static function get_mysql_bin($dbconf)
    {/*{{{*/
        return "mysql".
            " -h".$dbconf['host'].
            " -P".$dbconf['port'].
            " -u".$dbconf['user'].
            " -p".$dbconf['pass'].
            " --default-character-set=utf8";
    }/*}}}*/

    // 获取dbconf,统计库会根据dbprefix配置自动按日分库
    public static function get_dbconf($db)
    {/*{{{*/
        $dbconf = C::getConf($db);
        if (empty($dbconf)) {return;}
        $dbname = $db;
        if (isset($dbconf['db'])) $dbname=$dbconf['db'];
        else if (isset($dbconf['dbprefix'])) $dbname=$dbconf['dbprefix'].timeutil::getStatDay();
        $dbconf['db'] = $dbname;
        return $dbconf;
    }/*}}}*/

    // 创建database
    public static function create_database($db)
    {/*{{{*/
        //1. 获取db配置
        $dbconf = self::get_dbconf($db);
        if (empty($dbconf)) {return;}
        $dbname = $dbconf['db'];

        //2. create
        $sql = "CREATE DATABASE IF NOT EXISTS $dbname";
        $cmd = self::get_mysql_bin($dbconf)." -N -e \"$sql\"";
        runcmd($cmd);
    }/*}}}*/

    // 删除database
    public static function drop_database_timeout($db)
    {/*{{{*/
        //1. 获取db配置
        $dbconf = self::get_dbconf($db);
        if (empty($dbconf)) {return;}
        //2. 计算过期日期
        $savedays = intval($dbconf['savedays']);
        if ($savedays<=0) return;
        $timestamp = timeutil::getStatTime();
        $ts = $timestamp - $savedays*86400;
        $dbname = $dbconf['dbprefix'].date('Ymd',$ts);
        //3. drop database
        $sql = "DROP DATABASE IF EXISTS $dbname";
        $cmd = self::get_mysql_bin($dbconf)." -N -e \"$sql\"";
        runcmd($cmd);
    }/*}}}*/

    // 获取mysql表的建表语句
    public static function get_create_table_sql($db,$table)
    {/*{{{*/
        //1. 导出建表语句到文件
        $tmpfile = fileutil::genFileName("$db.$table.create.sql");
        self::export_file($db,"SHOW CREATE TABLE $table \G",$tmpfile);
        $res = file_get_contents($tmpfile);

        //2. 处理建表语句,发现不同mysql版本支持的建表语句不同,这里做下适配
        $pos = strpos($res,'CREATE TABLE');
        $res = substr($res, $pos);
        $res = str_replace('CREATE TABLE','CREATE TABLE IF NOT EXISTS', $res);
        $res = str_replace('ON UPDATE CURRENT_TIMESTAMP','', $res);
        $res = str_replace('DEFAULT CURRENT_TIMESTAMP','', $res);
        $res = str_replace('COLLATE utf8_bin','', $res);  //!< 去除二进制显示
        $res = str_replace('COLLATE=utf8_bin','', $res);  //!< 去除二进制显示
        $res = str_replace("\n",'', $res);
        $res = str_replace("\r",'', $res);
        $res = preg_replace("/innodb/i", "MyISAM", $res);
        return $res;
    }/*}}}*/

    // 导出数据到文件
    public static function export_file($db,$sql,$file)
    {/*{{{*/
        $dbconf = self::get_dbconf($db);
        if (empty($dbconf)) {return;}
        $dbname = $dbconf['db'];
        if (!dis_file($file)) {
            $cmd = self::get_mysql_bin($dbconf)." -N -e \"use $dbname;$sql\" > $file";
            runcmd($cmd);
        }
    }/*}}}*/
    
    // 导入文件到数据表
    public static function import_file($file,$db,$table,$replace=false)
    {/*{{{*/
        //1. 判断文件是否存在
        if (!dis_file($file)) {
            ublog::warning("file_not_exist[$file]");
            return;
        }
        //2. 导入数据
        $dbconf = self::get_dbconf($db);
        if (empty($dbconf)) {return;}
        $dbname = $dbconf['db'];
        $rp = $replace ? 'replace' : 'ignore';
        $sql = "load data local infile '$file' $rp into table $table";
        $cmd = self::get_mysql_bin($dbconf)." -N -e \"use $dbname;$sql\"";
        runcmd($cmd);
    }/*}}}*/
}
// vim600: sw=4 ts=4 fdm=marker syn=php
?>
