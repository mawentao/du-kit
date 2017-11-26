<?php

/**
 * 定义一些常量
 * mawentao(mawentao@baidu.com)
 **/


define("MT_LIB_PATH", realpath(dirname(__FILE__)));
define("MT_EXT_PATH", MT_LIB_PATH."/ext");
define("MT_CORE_PATH", MT_LIB_PATH."/mt-core");
define("MT_COMMON_PATH", MT_LIB_PATH."/mt-common");
define("MT_MYSQLDB_PATH", MT_LIB_PATH."/mt-mysqldb");

require_once(MT_CORE_PATH."/loader.php");
require_once(MT_CORE_PATH."/app.php");
require_once(MT_CORE_PATH."/model.php");
require_once(MT_CORE_PATH."/render.php");
require_once(MT_CORE_PATH."/ublog.php");
require_once(MT_CORE_PATH."/timer.php");
require_once(MT_CORE_PATH."/validate.php");

require_once(MT_MYSQLDB_PATH."/dbfunctions.php");
require_once(MT_MYSQLDB_PATH."/DaoFactory.php");
require_once(MT_MYSQLDB_PATH."/DBO.php");
require_once(MT_MYSQLDB_PATH."/DAO.php");

require_once(MT_COMMON_PATH."/UBClient.php");
require_once(MT_COMMON_PATH."/RedisClient.php");
require_once(MT_COMMON_PATH."/myphpfunc.php");


function mtinfo()
{
	echo "<h3>MT Version: 1.2 </h3>";
	echo "<hr>";

	$infolist = array (
		"MT_LIB_PATH" => MT_LIB_PATH,
		"MT_EXT_PATH" => MT_EXT_PATH,
		"MT_CORE_PATH" => MT_CORE_PATH,
		"MT_COMMON_PATH" => MT_COMMON_PATH,
		"MODEL_PATH" => (defined("MODEL_PATH") ? MODEL_PATH : "not defined"),
		"VIEW_PATH" => (defined("VIEW_PATH") ? VIEW_PATH : "not defined"),
		"CONTROLLER_PATH" => (defined("CONTROLLER_PATH") ? CONTROLLER_PATH : "not defined"),
	);

	foreach ($infolist as $k => $v) {
		echo "$k: $v <br>";
	}
}

?>
