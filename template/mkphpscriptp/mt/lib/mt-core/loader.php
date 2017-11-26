<?php

/***********************************************************
 * @file:   loader.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2013-09-09 18:44:42
 * @modify: 2013-09-09 18:44:42
 * @brief:  ×Ô¶¯¼ÓÔØclass
 ***********************************************************/

function class_loader($class_name)
{
	if (class_exists($class_name, false)) {
		return;
	}

	$class_name     = str_replace('_', '/', $class_name);
	$include_pathes = explode(PATH_SEPARATOR, get_include_path());
	foreach ($include_pathes as $path) {
		$real_class_file = $path."/$class_name.php";
		if (is_file($real_class_file)) {
			break;
		}
	}

    include $real_class_file;
}


// autoloader start
spl_autoload_register('class_loader');
// autoloader end

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
