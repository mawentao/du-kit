<?php

/***********************************************************
 * @file:   UBLog.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2013-07-24 14:11:48
 * @modify: 2013-07-24 14:11:48
 *          2014-07-17 14:24:48 ����printѡ��
 * @brief:  ��־ģ��
 ***********************************************************/

class UBLog
{
	// ��־����
	const LOG_FATAL   = 0;
	const LOG_WARNING = 2;
	const LOG_NOTICE  = 4;
	const LOG_TRACE   = 8;
	const LOG_DEBUG   = 16;

    // ��־�иʽ
	const SLICE_NONE  = 0;   //!< ���и�
    const SLICE_MONTH = 1;   //!< �����и�
    const SLICE_DAY   = 2;   //!< �����и�
	const SLICE_HOUR  = 3;   //!< ��Сʱ�и�

	static $LOG_NAME = array(
        self::LOG_FATAL   => 'FATAL',
        self::LOG_WARNING => 'WARNING', 
        self::LOG_NOTICE  => 'NOTICE', 
        self::LOG_TRACE   => 'TRACE', 
        self::LOG_DEBUG   => 'DEBUG',
	);

	private $_logname   = "";
	private $_logfile   = "";
	private $_logwffile = "";
	private $_loglevel  = self::LOG_DEBUG;
	private $_slice     = self::SLICE_NONE;
	private $_print     = false;

    /**
     * ���캯��
     */
    public function __construct ($arrConfig)
    {
		$this->_logname   = strtoupper($arrConfig['logname']);
		$this->_logfile   = $arrConfig['logpath']."/".$arrConfig['logname'].".log";
		$this->_logwffile = $arrConfig['logpath']."/".$arrConfig['logname'].".log.wf";
		$this->_loglevel  = $arrConfig['loglevel'];
		$this->_slice     = $arrConfig['slice'];

		$this->_print     = isset($arrConfig['print']) ? $arrConfig['print'] : false;
    }

	/* ��¼��־�ӿ� */
    public function fatal()  { $arg=func_get_args(); $this->write_log(self::LOG_FATAL,$arg); }
    public function warning(){ $arg=func_get_args(); $this->write_log(self::LOG_WARNING,$arg); }
    public function notice (){ $arg=func_get_args(); $this->write_log(self::LOG_NOTICE,$arg); }
    public function trace()  { $arg=func_get_args(); $this->write_log(self::LOG_TRACE,$arg); }
    public function debug()  { $arg=func_get_args(); $this->write_log(self::LOG_DEBUG,$arg); }


	//=============������˽�к���==============

    private function write_log ($strType, $arr_data)
    {
		//1. ��־�������
		$loglevel = intval($strType);
		if($loglevel>$this->_loglevel || ($loglevel%2)===1){
			return;
		}

		//2. ������־���𣬻�ȡ��Ӧ����־�ļ�
		$logfile = $this->_logfile;
        if($loglevel <= self::LOG_WARNING) {
            $logfile = $this->_logwffile;
        }
        
		//3. �и��
		switch($this->_slice){
			case self::SLICE_NONE:  break;
			case self::SLICE_MONTH: $logfile.=".".date('Ym'); break;
			case self::SLICE_DAY:   $logfile.=".".date('Ymd'); break;
			case self::SLICE_HOUR:  $logfile.=".".date('Ymd-H'); break;
		}

		//4. ��־����
		$str = sprintf("%s\t%s\t", self::$LOG_NAME[$loglevel], date("m-d H:i:s"), $this->_logname);
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
        
		//5. д��־
		$this->write_file($logfile, $strLog."\n");
    }

    private function write_file ($strPath, $str)
    {
        @error_log($str, 3, $strPath);
		if ($this->_print) {
			echo $str;
		}
        return;
    }
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
