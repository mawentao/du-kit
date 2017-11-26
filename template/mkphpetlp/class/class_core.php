<?php
require_once libfile('class/rollback');
require_once libfile('class/mysql_util');
require_once libfile('class/dbo');
require_once libfile('class/mongo');

class core
{
    private static $_conf_objs;
    private static $_dbo_objs;
    private static $_mongo_objs;
    private static $_model_objs;

    // 初始化
    public static function init()
    {
        global $argv;

        //1. 初始化日志模块
        $conf = self::conf('main');
        require_once libfile('class/ublog');
        ublog::init($conf['log']);

        //2. 时间模块初始化
        require_once libfile('class/timeutil');
        $arg = isset($argv[2]) ? $argv[2] : '';
        timeutil::init($arg);
        ublog::trace("StatDay: ".timeutil::getStatDay());

        // 初始化文件模块
        require_once libfile('class/fileutil');
        fileutil::init($conf['file']['data_dir']);

        // 初始化DB
        require_once libfile('class/dbdump');
        dbdump::init($conf);
    }

    // 获取model对象实例
    public static function m($modelname)
    {
        if (!isset(self::$_model_objs[$modelname])) {
            require_once libfile("model/$modelname");
            self::$_model_objs[$modelname] = new $modelname();
        } 
        return self::$_model_objs[$modelname];
    }


    // 获取db访问对象(pdo实例)
    public static function dbo($dbname)
    {
        if (!isset(self::$_dbo_objs[$dbname])) {
            $mysqlconf = mysql_util::get_dbconf($dbname);
            if (!empty($mysqlconf)) {
                $host = $mysqlconf["host"];
                $port = $mysqlconf["port"];
                $user = $mysqlconf["user"];
                $pass = $mysqlconf["pass"];
                $db = $mysqlconf["db"];
                self::$_dbo_objs[$dbname] = new DBO($host,$port,$user,$pass,$db);
            }
        }
        return self::$_dbo_objs[$dbname];
    }

    // 获取mongo访问对象
    public static function mongo($dbname)
    {   
        $conf = self::conf('main');
        if (!isset(self::$_mongo_objs[$dbname])) {
            $mysqlconf = $conf[$dbname];
            $host = $mysqlconf["host"];
            $port = $mysqlconf["port"];
            $user = $mysqlconf["user"];
            $pass = $mysqlconf["pass"];
            $db = $mysqlconf["db"];
            self::$_mongo_objs[$dbname] = new MongoDBO($host,$port,$user,$pass,$db);
        }   
        return self::$_mongo_objs[$dbname];
    }

    // 获取配置文件
    public static function conf($confname)
    {
        if (!isset(self::$_conf_objs[$confname])) {
            $confpath = dirname(dirname(__FILE__))."/conf";
            $pi = pathinfo($confname);
            if (!isset($pi["extension"]) || strtolower($pi["extension"]) != "ini") {
                $confname .= ".ini";
            }
            $conffile = $confpath."/".$confname;
            if (is_file($conffile)) {
                self::$_conf_objs[$confname] = parse_ini_file($conffile, true);
            } else {
                self::$_conf_objs[$confname] = array();
            }
        }
        return self::$_conf_objs[$confname];   
    }

    // 根据key获取配置
    public static function getConf($key,$confname='main')
    {
        $conf = self::conf($confname);
        if (!empty($conf) && isset($conf[$key])) {
            return $conf[$key];
        }
        ublog::warning("cannot find conf[$key] in file [$confname.ini]");
        return array();
    }
}

class C extends core {}

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
