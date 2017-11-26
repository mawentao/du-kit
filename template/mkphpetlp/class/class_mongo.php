<?php
/***********************************************************
 * @file:   class_mongo.php
 * @author: mawentao
 * @create: 2016-12-05 13:33:40
 * @modify: 2016-12-05 13:33:40
 * @brief:  访问MongoDB的简单封装
 ***********************************************************/

class MongoDBO
{
	private $obj=null;

	/* 构造函数 */
	public function __construct($host, $port, $user, $pass, $db)
	{
		$mongocon = new MongoClient("mongodb://$user:$pass@$host:$port");
		$this->obj = $mongocon->selectDB($db);
	}

	/* 获取DB */
	public function getdb() {
		return $this->obj;
	}

	// 获取字段key为某些值的所有文档
	public function fetch_all_in($key, array $values)
	{
		
	}
	
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
