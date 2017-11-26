<?php
if (!defined('IN__HEADFILENAME__API')) {
    exit('Access Denied');
}
/**
 * 图文验证码
 * URL: dzroot/source/plugin/_FILENAME_/index.php?version=1&module=seccode
 **/
require './source/class/class_core.php';
$discuz = C::app();
$discuz->init();
require_once _HEADFILENAME__PLUGIN_PATH."/class/env.class.php";
$sc = C::m('#_FILENAME_#_FILENAME__seccode');
/*
//debug
if (isset($_GET['seccode'])) {
	var_dump($sc->check($_GET['seccode']));
	die(0);
}
//*/
$code = $sc->mkcode(4,false);
$sc->display($code,120,40);
?>
