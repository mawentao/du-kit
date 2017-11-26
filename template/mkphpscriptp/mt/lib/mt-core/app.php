<?php

/***********************************************************
 * @file:   app.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2013-09-09 16:51:23
 * @modify: 2013-09-09 16:51:23
 * @brief:  UI Framework
 ***********************************************************/

class UI_Application
{
	public function __construct($request = null)
	{	
		//1. 解析web参数
		Env::parseWeb();

		//2. 打印请求日志
		/*
		if(isset($_SERVER['REQUEST_URI'])) {
			$request_uri = $_SERVER['REQUEST_URI'];
		} else if(isset($_SERVER['QUERY_STRING'])) {
			$request_uri = $_SERVER['QUERY_STRING'];
		} else {
			$request_uri = $_SERVER['PHP_SELF'];
		}	
		Env::getlog()->notice('RequestLog', $request_uri);
		*/
	}

	/* 根据类名和函数名创建对象 */
	private function createObj($className, $functionName, &$obj)
	{/*{{{*/
		if (!class_exists($className)) {
			return false;
		}
		$obj = new $className;
		if (!method_exists($obj, $functionName)) {
			return false;
		}
		return true;
	}/*}}}*/

	/* 获取待运行的类名及运行函数名 */
	private function getClassAndFunction(&$obj, &$functionName)
	{/*{{{*/
		$controllerName = Env::getController();
		$actionName     = Env::getAction();

		//1. 从Controller文件中获取
		$file = CONTROLLER_PATH."/".ucfirst($controllerName).".php";
		if (is_file($file)) {
			include_once($file);
			$className    = ucfirst($controllerName).'Controller';
			$functionName = $actionName."Action";
			if ($this->createObj($className, $functionName, $obj)) {
				return true;
			}
		} 

		//2. 如果Controller文件不存在，则从Action文件中获取
		$file = CONTROLLER_PATH."/".$controllerName."/".$actionName."Action.php";
		if (is_file($file)) {
			include_once($file);
			$className    = ucfirst($actionName)."Action";
			$functionName = "execute";
			if ($this->createObj($className, $functionName, $obj)) {
				return true;
			}
		}

		return false;
	}/*}}}*/

	/* 运行APP */
	public function run()
	{
		//1. 获取执行的类和函数
		if ($this->getClassAndFunction($obj, $fun) === false) {
			$controller = Env::getController();
			$action = Env::getAction();
			Env::$resData["errmsg"] = "Can not find controller[ <b>$controller</b> ] or action[ <b>$action</b> ].";
			$this->fatal();
		}
		
		//2. 执行函数	
		try {
			//2-1. 初始化
			$obj->init();

			//2-2. 检查请求参数
			$errmsg = "";
			if (!$obj->checkRequest($errmsg)) {
				if ($errmsg == "") {
					$errmsg = "非法请求";
				}
				if ($obj->_render == 'ajax') {
					Env::$resData["retcode"] = -1;
					Env::$resData["retmsg"]  = $errmsg;
					Render::renderResult($obj->_render, $obj->_template);
					return;
				} else {
					Env::$resData["errmsg"] = $errmsg;
					$this->fatal();
				}
			}

			//2-3. 执行请求
			$obj->$fun();

			//2-4. 渲染输出数据
			Render::renderResult($obj->_render, $obj->_template);

		} catch (Exception $e) {	
			Env::getlog()->fatal($e->getMessage());
			Env::$resData["errmsg"] = $e->getMessage();
			$this->fatal();
		}
	}


	// 终止
	private function fatal()
	{
		require_once(MT_LIB_PATH."/mt-core/errpage.php");
		exit(-1);
	}
}

//=============================================================

abstract class UI_Controller
{
	public $_template = "";      //!< smarty模块文件
	public $_render   = 'php';   //!< 默认输出数据的渲染方式

	// 初始化操作
	public function init() {}

	// 设置输出数据渲染方式
    public function setRender($render)
	{
		$this->_render = $render;
	}
    
	// 设置tpl模块文件
	public function setTemplate($tpl)
	{
		$this->_template = $tpl;
	}


	/* 检查请求参数 */
	public function checkRequest(&$errmsg)
	{
		if (Validate::checkAttackRequest())
			return true;
		$errmsg = "非法请求";
		return false;
	}

}


abstract class UI_Action extends UI_Controller
{
	public $_render = 'ajax';   //!< 默认输出数据的渲染方式	


	/* 执行Action */
	public function execute()
	{
	}

}

////////////////////////////////////////////////////////
// Query类型的Action抽象
class Query_UI_Action extends UI_Action
{
    /* 检查请求参数 */
    public function checkRequest(&$errmsg)
    {/*{{{*/
        if (!parent::checkRequest($errmsg)) {
            return false;
        }

		try {
			Validate::checkOPParameter("start", "start", "integer", 32, 0);
			Validate::checkOPParameter("limit", "limit", "integer", 32, 100);
			Validate::checkOPParameter("sort", "sort", "string", 32, "");
			Validate::checkOPParameter("dir", "dir", "string", 32, "DESC");
			
			return true;
		} catch (Exception $e) {
			$errmsg = $e->getMessage();
			return false;
		}
    }/*}}}*/

    // 执行查询
    public function query(&$pdo, $table, $columns, $query_cond)
    {/*{{{*/
        $start = $_REQUEST["start"];
        $limit = $_REQUEST["limit"];
        $sort = $_REQUEST["sort"];
        $dir = $_REQUEST["dir"];

		try { 
			//1. 查询总记录数
			$sql = "select count(*) as num from $table where $query_cond";
			$smt = $pdo->query($sql);
			$total_found = 0;
			if ($smt) {
				$arr = $smt->fetchAll();
				$total_found = $arr[0]["num"];
			}
			$smt = null;

			//2. 查询当前页
			$ordersmt = "";
			if ($sort != "") {
				$ordersmt = "order by $sort $dir";
			}
			$sql  = "select $columns from $table where $query_cond $ordersmt limit $start, $limit";
			$smt  = $pdo->query($sql);
			$data = array();
			while($smt && $row = $smt->fetch()) {
				array_unset_int_key($row);
				$data[] = $row;
			}
			$smt = null;

			Env::$resData["totalProperty"] = $total_found;
			Env::$resData["root"] = &$data;
		} catch (Exception $e) {
			Env::$resData["totalProperty"] = 0;
            Env::$resData["root"] = array();
			Env::$resData["retmsg"] = $e->getMessage();
		}
    }/*}}}*/
	
	/* 执行Action */
	public function execute()
	{
	}
}


////////////////////////////////////////////////////////


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
