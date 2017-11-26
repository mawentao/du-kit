<?php
if(!defined('IN_DISCUZ') || !defined('IN_ADMINCP')) {
    exit('Access Denied');
}
require_once dirname(__FILE__).'/class/env.class.php';

$params = array (
	'siteurl' => _FILENAME__env::get_siteurl(),
);
$tplVars = array(
    'siteurl'     => _FILENAME__env::get_siteurl(),
    'plugin_path' => _FILENAME__env::get_plugin_path(),
	'plugin_dir'  => dirname(__FILE__),
	'php_bin'     => PHP_BINDIR."/php",
);
_FILENAME__utils::loadtpl(dirname(__FILE__).'/template/views/z_about.tpl', $params, $tplVars);
_FILENAME__env::getlog()->trace("show admin page [z_about] success");
