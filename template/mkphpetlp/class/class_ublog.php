<?php
/**
 * 日志模块
 **/
class ublog     
{               
    // 日志级别
    const LOG_FATAL   = 0;
    const LOG_WARNING = 2; 
    const LOG_NOTICE  = 4;
    const LOG_TRACE   = 8;
    const LOG_DEBUG   = 16;
        
    // 日志切割方式
    const SLICE_NONE  = 0;   //!< 不切割
    const SLICE_MONTH = 1;   //!< 按月切割
    const SLICE_DAY   = 2;   //!< 按日切割
    const SLICE_HOUR  = 3;   //!< 按小时切割

    static $LOG_NAME = array(
        self::LOG_FATAL   => 'FATAL',
        self::LOG_WARNING => 'WARNING',
        self::LOG_NOTICE  => 'NOTICE',
        self::LOG_TRACE   => 'TRACE',
        self::LOG_DEBUG   => 'DEBUG',
    );  
    
    private static $_logname   = "";
    private static $_logfile   = "";
    private static $_logwffile = "";
    private static $_loglevel  = self::LOG_DEBUG;
    private static $_slice     = self::SLICE_NONE;
    private static $_print     = false;

    // 初始化日志模块
    public static function init($arrConfig)
    {
        self::$_logname   = strtolower($arrConfig['log_name']);
        self::$_loglevel  = $arrConfig['log_level'];
        self::$_slice     = $arrConfig['auto_slice'];
        self::$_print     = isset($arrConfig['print']) ? $arrConfig['print'] : false;
        //1. 初始化日志路径
        $log_path = $arrConfig['log_path']; 
        if (!is_dir($log_path)) { mkdir($log_path,0777,true); }
        //2. 日志切割方案
        $logfileprefix = $log_path."/".self::$_logname;
        $logfilesubfix = "";
        switch(self::$_slice){
            case self::SLICE_NONE:  break;
            case self::SLICE_MONTH: $logfilesubfix=".".date('Ym'); break;
            case self::SLICE_DAY:   $logfilesubfix=".".date('Ymd'); break;
            case self::SLICE_HOUR:  $logfilesubfix=".".date('Ymd-H'); break;
        }
        //3. 初始化日志文件
        self::$_logfile   = $logfileprefix.".log".$logfilesubfix;
        self::$_logwffile = $logfileprefix.".log.wf".$logfilesubfix;
        if (!is_file(self::$_logfile)) touch(self::$_logfile);
        if (!is_file(self::$_logwffile)) touch(self::$_logwffile);
    }

    /* 记录日志接口 */
    public static function fatal()  { $arg=func_get_args(); self::write_log(self::LOG_FATAL,$arg); }
    public static function warning(){ $arg=func_get_args(); self::write_log(self::LOG_WARNING,$arg); }
    public static function notice (){ $arg=func_get_args(); self::write_log(self::LOG_NOTICE,$arg); }
    public static function trace()  { $arg=func_get_args(); self::write_log(self::LOG_TRACE,$arg); }
    public static function debug()  { $arg=func_get_args(); self::write_log(self::LOG_DEBUG,$arg); }


    //=============以下是私有函数==============

    private static function write_log ($strType, $arr_data)
    {
        //1. 日志级别过滤
        $loglevel = intval($strType);
        if($loglevel>self::$_loglevel || ($loglevel%2)===1){
            return;
        }

        //2. 根据日志级别，获取相应的日志文件
        $logfile = self::$_logfile;
        if($loglevel <= self::LOG_WARNING) {
            $logfile = self::$_logwffile;
        }

        //3. 日志内容
        $str = sprintf("[%s]\t(%s)\t", self::$LOG_NAME[$loglevel], date("m-d H:i:s"), self::$_logname);
        if(!empty($arr_data)) {
            $str .= implode("\t",$arr_data);
        }
        $strLog = '';
        switch ($strType) {
            case self::LOG_WARNING:
            case self::LOG_FATAL:
            case self::LOG_DEBUG:
                $debug_info = debug_backtrace();
                $strLog .= "[{$debug_info[1]['file']}" . ":" . $debug_info[1]['line'] . "]\t";
                $strLog .= $str;
                break;
            case self::LOG_TRACE:
            case self::LOG_NOTICE:
                $strLog .= $str;
                break;
            default:
                break;
        }
        $strLog = str_replace("\n","",$strLog);

        //5. 写日志
        self::write_file($logfile, $strLog."\n");
    }

    private static function write_file ($strPath, $str)
    {
        @error_log($str, 3, $strPath);
        if (self::$_print) {
            echo $str;
        }
        return;
    }
}

?>
