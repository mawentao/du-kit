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
		//1. ����web����
		Env::parseWeb();

		//2. ��ӡ������־
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

	/* ���������ͺ������������� */
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

	/* ��ȡ�����е����������к����� */
	private function getClassAndFunction(&$obj, &$functionName)
	{/*{{{*/
		$controllerName = Env::getController();
		$actionName     = Env::getAction();

		//1. ��Controller�ļ��л�ȡ
		$file = CONTROLLER_PATH."/".ucfirst($controllerName).".php";
		if (is_file($file)) {
			include_once($file);
			$className    = ucfirst($controllerName).'Controller';
			$functionName = $actionName."Action";
			if ($this->createObj($className, $functionName, $obj)) {
				return true;
			}
		} 

		//2. ���Controller�ļ������ڣ����Action�ļ��л�ȡ
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

	/* ����APP */
	public function run()
	{
		//1. ��ȡִ�е���ͺ���
		if ($this->getClassAndFunction($obj, $fun) === false) {
			$controller = Env::getController();
			$action = Env::getAction();
			Env::$resData["errmsg"] = "Can not find controller[ <b>$controller</b> ] or action[ <b>$action</b> ].";
			$this->fatal();
		}
		
		//2. ִ�к���	
		try {
			//2-1. ��ʼ��
			$obj->init();

			//2-2. ����������
			$errmsg = "";
			if (!$obj->checkRequest($errmsg)) {
				if ($errmsg == "") {
					$errmsg = "�Ƿ�����";
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

			//2-3. ִ������
			$obj->$fun();

			//2-4. ��Ⱦ�������
			Render::renderResult($obj->_render, $obj->_template);

		} catch (Exception $e) {	
			Env::getlog()->fatal($e->getMessage());
			Env::$resData["errmsg"] = $e->getMessage();
			$this->fatal();
		}
	}


	// ��ֹ
	private function fatal()
	{
		require_once(MT_LIB_PATH."/mt-core/errpage.php");
		exit(-1);
	}
}

//=============================================================

abstract class UI_Controller
{
	public $_template = "";      //!< smartyģ���ļ�
	public $_render   = 'php';   //!< Ĭ��������ݵ���Ⱦ��ʽ

	// ��ʼ������
	public function init() {}

	// �������������Ⱦ��ʽ
    public function setRender($render)
	{
		$this->_render = $render;
	}
    
	// ����tplģ���ļ�
	public function setTemplate($tpl)
	{
		$this->_template = $tpl;
	}


	/* ���������� */
	public function checkRequest(&$errmsg)
	{
		if (Validate::checkAttackRequest())
			return true;
		$errmsg = "�Ƿ�����";
		return false;
	}

}


abstract class UI_Action extends UI_Controller
{
	public $_render = 'ajax';   //!< Ĭ��������ݵ���Ⱦ��ʽ	


	/* ִ��Action */
	public function execute()
	{
	}

}

////////////////////////////////////////////////////////
// Query���͵�Action����
class Query_UI_Action extends UI_Action
{
    /* ���������� */
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

    // ִ�в�ѯ
    public function query(&$pdo, $table, $columns, $query_cond)
    {/*{{{*/
        $start = $_REQUEST["start"];
        $limit = $_REQUEST["limit"];
        $sort = $_REQUEST["sort"];
        $dir = $_REQUEST["dir"];

		try { 
			//1. ��ѯ�ܼ�¼��
			$sql = "select count(*) as num from $table where $query_cond";
			$smt = $pdo->query($sql);
			$total_found = 0;
			if ($smt) {
				$arr = $smt->fetchAll();
				$total_found = $arr[0]["num"];
			}
			$smt = null;

			//2. ��ѯ��ǰҳ
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
	
	/* ִ��Action */
	public function execute()
	{
	}
}


////////////////////////////////////////////////////////


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
