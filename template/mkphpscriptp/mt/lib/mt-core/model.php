<?php

/***********************************************************
 * @file:   model.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2014-08-12 11:29:17
 * @modify: 2014-08-12 11:29:17
 * @brief:  model.php
 ***********************************************************/

/**
 * 所有Model的父类 (模板方法设计模式)
 **/
class UI_Model
{
	protected $model_name = __CLASS__;
	protected $model_data = array();

	public function __construct() { }

	public final function pack()
	{
		$this->execute();
		Env::$resData[$this->model_name] = &$this->model_data;
	}

	protected function execute()
	{
	}
}


/**
 * pack envData
 **/
class Env_Model extends UI_Model
{
	public function __construct() 
	{ 
		$this->model_name = "envData";
	} 

	protected function execute()
	{
		$this->model_data = &Env::$envData;
	}
}




if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME']))
{
	// test
	require_once("../env.php");
	Env::getmodel("Env_Model")->pack();
	print_r(Env::$resData);

	echo "!!!THE END!!!\n";
}



// vim600: sw=4 ts=4 fdm=marker syn=php
?>
