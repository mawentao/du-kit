<?php
/***********************************************************
 * @file:   Env.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2013-08-21 09:12:43
 * @modify: 2013-08-21 09:12:43
 *          2014-07-17 14:13:20 ����webp��phptool
 * @brief:  ������Դ�࣬�ṩ���巶Χ�ڵĹ�����Դ
 ***********************************************************/

require_once "def.php";

/**
 * ȫ�ֻ�����Դ�࣬�������ȫ�����е�������Դ
 **/
final class Env
{
	////////////////////////////////////////////////////////
    // variables
	////////////////////////////////////////////////////////
	public static $envData = array();    //!< ��������
	private static $_isDebug = false;    //!< Debugģʽ����

	private static $diContainer = array();
	private static $sharedContainer = array();

	private static $logobjs = array();
	private static $logconf = array(
		"log_path"   => "./log",
		"log_name"   => "main",
		"log_level"  => UBLog::LOG_DEBUG,
		"auto_slice" => UBLog::SLICE_NONE,
		"print"      => false,
	);

	private static $ubcobjs  = array();  //!< UBClientʵ��
	private static $pdoobjs  = array();  //!< PDOʵ��
	private static $models   = array();  //!< Modelʵ��
	private static $confobjs = array();  //!< �����ļ�ʵ��

	public static $uilog = "";          //!< һ�ν�����log
	public static $resData = array();   //!< �Ự��������ݴ���ڴ˱����� 
	public static $tmpData = array();   //!< �Ự��������в�������ʱ���ݴ���ڴ˱�����

	////////////////////////////////////////////////////////
    // functions
	////////////////////////////////////////////////////////

	public static function setDebug($isDebug=true) { self::$_isDebug = $isDebug;}
	public static function isDebug() { return self::$_isDebug; }
	
    /* ��ʼ��������Դ */
	public static function init($logconf=null)
	{
		//1. ��ʼ����־����
		if ($logconf) {
			if (isset($logconf["log_path"])) self::$logconf["log_path"] = $logconf["log_path"];
			if (isset($logconf["log_name"])) self::$logconf["log_name"] = $logconf["log_name"];
			if (isset($logconf["log_level"])) self::$logconf["log_level"] = $logconf["log_level"];
			if (isset($logconf["auto_slice"])) self::$logconf["auto_slice"] = $logconf["auto_slice"];
			if (isset($logconf["print"])) self::$logconf["print"] = $logconf["print"];
		}
		if (!is_dir(self::$logconf["log_path"])) {
			mkdir(self::$logconf["log_path"], 0777, true);
		}
	}

	///////////////////////////////////////////////////////
	// for script tool
	private static function printUsage($pname)
	{/*{{{*/
		printf("[usage]: $pname input_file [-n line_num] [-o output_file]\n");
		exit(0);
	}/*}}}*/
	
	// ���������в���
    public static function parseArgs()
    {/*{{{*/
        global $argc, $argv;
        if($argc < 2){ self::printUsage($argv[0]); }

        //1. ���������в���
        $g_conf = &self::$envData;
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
                self::printUsage($argv[0]);
            }       
            switch($state){
                case 0: $g_conf["input_file"] = $argv[$i]; break;
                case 1: $g_conf["line_num"] = intval($argv[$i]); break;
                case 2: $g_conf["output_file"] = $argv[$i]; break;
            }
        }   

        //2. ������������
        if($g_conf["input_file"] == ""){self::printUsage($argv[0]);}
        if($g_conf["output_file"]== ""){
            $g_conf["output_file"] = $g_conf["input_file"].".xls";
        }       
        if($g_conf["line_num"] == 0){
            $g_conf["line_num"] = count_line_num($g_conf["input_file"]);
        }
    }/*}}}*/

	/////////////////////////////////////////
	// for web
    // ����web����
	public static function parseWeb()
	{/*{{{*/
		//1. ����appurl,pageurl,controller,action
		$appurl  = $_SERVER["SCRIPT_NAME"];
		$rui =  $_SERVER["REQUEST_URI"];
		$pos = strpos($rui, "?");
		if($pos !== FALSE){
			$rui = substr($rui, 0, $pos);
		}
		$pageurl = $rui;
		$prefix  = $appurl;
		if (strlen($appurl) > strlen($pageurl)) {
			$prefix  = $pageurl;
			$pageurl = $appurl;
		}
		$rui = str_replace($prefix, "", $rui);
		$rui = trim($rui, "/");
		$arr = explode("/", $rui);

		$controller = (isset($arr[0]) && $arr[0]!="") ? $arr[0] : "index";
		$controller = strtolower($controller);
		$action     = (isset($arr[1]) && $arr[1]!="") ? $arr[1] : "index";
		$action     = strtolower($action);

		self::$envData["APP_URL"]    = $appurl;     //!< WebApp URL
		self::$envData["PAGE_URL"]   = $pageurl;    //!< ��ǰҳURL�����������������
		self::$envData["CONTROLLER"] = $controller; //!< ��������ȫСд��
		self::$envData["ACTION"]     = $action;     //!< action��ȫСд��

		//2. ����cookie
		if (isset($_SERVER["HTTP_COOKIE"])) {
			$arr = explode("=",$_SERVER["HTTP_COOKIE"]);
			self::$envData["COOKIE"] = end($arr);
		} else {
			self::$envData["COOKIE"] = md5(microtime());
		}
	}/*}}}*/

	public static function getAppurl()     { return self::$envData["APP_URL"]; }
	public static function getPageurl()    { return self::$envData["PAGE_URL"]; }
	public static function getController() { return self::$envData["CONTROLLER"]; }
	public static function getAction()     { return self::$envData["ACTION"]; }
	public static function getCookie()     { return self::$envData["COOKIE"]; }

	// ��ȡ�ͻ���IP
	public static function getIP()
	{/*{{{*/
		$uip = ''; 
		if(getenv('HTTP_X_FORWARDED_FOR') && strcasecmp(getenv('HTTP_X_FORWARDED_FOR'), 'unknown')) {
			$uip = getenv('HTTP_X_FORWARDED_FOR');
			strpos($uip, ',') && list($uip) = explode(',', $uip);
		} else if(getenv('HTTP_CLIENT_IP') && strcasecmp(getenv('HTTP_CLIENT_IP'), 'unknown')) {
			$uip = getenv('HTTP_CLIENT_IP');
		} else if(getenv('REMOTE_ADDR') && strcasecmp(getenv('REMOTE_ADDR'), 'unknown')) {
			$uip = getenv('REMOTE_ADDR');
		} else if(isset($_SERVER['REMOTE_ADDR']) && $_SERVER['REMOTE_ADDR'] 
			&& strcasecmp($_SERVER['REMOTE_ADDR'], 'unknown')) 
		{
			$uip = $_SERVER['REMOTE_ADDR'];
		}   
		return $uip;
	}/*}}}*/

	// ��ȡ��������IP
	public static function getServerIP()
	{/*{{{*/
		if (isset($_SERVER['SERVER_ADDR']))
			return $_SERVER['SERVER_ADDR'];
		return '127.0.0.1'; 
	}/*}}}*/

	// ��ȡ������־
	public static function getIDC()
	{/*{{{*/
        $idc = explode('-', self::getHostName());
        $mapIDC = array(
            'ai' => 'dx',
            'yf' => 'dx',
            'jx' => 'dx',
            'hz01' => 'hz',
            'db' => 'wt',
            'cq01' => 'wt',
            'tc' => 'wt',
            'st01' => 'wt',
            'cq02' => 'cq02', 
        );      
        if (! isset($mapIDC[$idc[0]])) {
            $idc_real = 'dx'; 
        } else {
            $idc_real = $mapIDC[$idc[0]];
        }       
        return $idc_real;
	}/*}}}*/

	public static function getHostName()
	{/*{{{*/
		return gethostbyaddr($_SERVER['SERVER_ADDR']);
	}/*}}}*/

	// ����Զ�����class·��
	public static function appendLoaderPath($path) 
	{/*{{{*/
		set_include_path(get_include_path().PATH_SEPARATOR.$path);
	}/*}}}*/

	/** ��ȡ�����ļ�ʵ�� ���趨��CONF_PATH������ */
	public static function getconf($confname="main")
	{/*{{{*/
		if (!isset(self::$confobjs[$confname])) {
			$confpath = defined("CONF_PATH") ? CONF_PATH : "./conf";
			$pi = pathinfo($confname);
			if (!isset($pi["extension"]) || strtolower($pi["extension"]) != "ini") {
				$confname .= ".ini";
			}
			$conffile = $confpath."/".$confname;
			if (is_file($conffile)) {
				self::$confobjs[$confname] = parse_ini_file($conffile, true);
			} else {
				self::$confobjs[$confname] = array();
			}
		}
		return self::$confobjs[$confname];
	}/*}}}*/

	/** ��ȡUBLogʵ�� */
    public static function getlog($name = NULL)
    {/*{{{*/
        if($name === NULL){
            $name = self::$logconf["log_name"];
        }

        if(!isset(self::$logobjs[$name])){
            $name_lower = strtolower($name);
			$ublog = new UBLog(array(
				"logname"  => $name_lower,
				"logpath"  => self::$logconf["log_path"],
				"loglevel" => self::$logconf["log_level"],
				"slice"    => self::$logconf["auto_slice"],
				"print"    => self::$logconf["print"],
			));
           self::$logobjs[$name] = $ublog;
        }
        return self::$logobjs[$name];
    }/*}}}*/

	/** ��ȡModelʵ�� */
	public static function getmodel($classname)
	{/*{{{*/
		if (!isset(self::$models[$classname])) {
			self::$models[$classname] = new $classname();
		}
		return self::$models[$classname];
	}/*}}}*/


	public static function set($name, $func)
    {/*{{{*/
        if (is_callable($func)) {
            self::$diContainer[$name] = $func;
        } else {
            self::$sharedContainer[$name] = $func;
        }
    }/*}}}*/
	
	public static function get($name)
    {/*{{{*/
        if (isset(self::$sharedContainer[$name])) {
            return self::$sharedContainer[$name];
        } elseif (isset(self::$diContainer[$name])) {
            $params = func_get_args();
            array_shift($params);
            return call_user_func_array(self::$diContainer[$name], $params);
        } else {
            Env::getlog()->warning('Could not find ' . $name . ' in DI container');
            return null;
        }
    }/*}}}*/

	// ע��һ��UBClient
	public static function setubc($key, $ubsvr_ips, $ubsvr_port, $timeout=0.1)
	{/*{{{*/
		if(!isset(self::$ubcobjs[$key])){
			self::$ubcobjs[$key] = new UBClient($ubsvr_ips, $ubsvr_port, $timeout);
		}
	}/*}}}*/

	// ��ȡUBClient
	public static function getubc($key)
	{/*{{{*/
		if(isset(self::$ubcobjs[$key])){
			return self::$ubcobjs[$key];
		}
		return NULL;
	}/*}}}*/

	// ע��һ��PDOʵ��
	public static function setpdo($mysqlconf, $key="MYSQL")
	{/*{{{*/
		if (!isset(self::$pdoobjs[$key])) {
			$host = $mysqlconf["host"];
			$port = $mysqlconf["port"];
			$user = $mysqlconf["user"];
			$pass = $mysqlconf["pass"];
			$db = $mysqlconf["db"];
			self::$pdoobjs[$key] = new DBO($host,$port,$user,$pass,$db);
		}
	}/*}}}*/

	// ��ȡPDOʵ��
	public static function getpdo($key="MYSQL")
	{/*{{{*/
		if (isset(self::$pdoobjs[$key])) {
			return self::$pdoobjs[$key];
		}
		return NULL;
	}/*}}}*/

	// ����AtomID
    public static function getAtomId($intType, $intNum=1)
    {/*{{{*/
        $addr = MT_EXT_EC_Conf::$ATOM_SVR["v2"];
        AtomClient::setOptions($addr);
        try {
            $proxy = new AtomClient();
            $ret = $proxy->getId($intType, $intNum);
            if ($intNum == 1) { 
                return $ret[0];
            } else {
                return $ret;
            }      
        } catch (Exception $e) {
            self::getlog()->warning("Get atom id failed. errmsg[" . $e->getMessage() . "]"); 
            return false;
        }   
    }/*}}}*/
	
	// ����word�����wordid
    public static function getWordidByWord(array $arrWord, $gen=false)
    {/*{{{*/
        $addr = MT_EXT_EC_Conf::$ATOM_SVR["v1"];
        AtomClient::setOptions($addr);
        try {
            $proxy = new AtomClient();
            $ret = $proxy->getWordidByWord($arrWord, $gen);
            return $ret;
        } catch (Exception $e) {
            self::getlog()->warning("Get atom id failed. errmsg[" . $e->getMessage() . "]");
            return false;
        }
    }/*}}}*/

	// ����wordid������
    public static function getWordByWordid(array $arrWordids)
    {/*{{{*/
        $addr = MT_EXT_Temai_Conf::$ATOM_SVR["v1"];
        AtomClient::setOptions($addr);
        try {
            $proxy = new AtomClient();
            $ret = $proxy->getWordByWordid($arrWordids);
            return $ret;
        } catch (Exception $e) {
            self::getlog()->warning("Get atom id failed. errmsg[" . $e->getMessage() . "]");
            return false;
        }
    }/*}}}*/

	// ��ȡurlǩ����64λ��ע�⣺���ʽ��url��
    public static function getUrlSign(&$url) { return SignSvr::signUrl($url); }
}



// vim600: sw=4 ts=4 fdm=marker syn=php
?>
