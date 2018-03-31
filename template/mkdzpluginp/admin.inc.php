<?php
if (!defined('IN_DISCUZ')) {
    exit('Access Denied');
}
require_once dirname(__FILE__)."/class/env.class.php";

// 登录检查
if(!$_G['uid']){
	$login = _FILENAME__env::get_siteurl()."/member.php?mod=logging&action=login";
    header("Location: $login");
    exit();
}
// 权限检查
$auth = C::t('#_FILENAME_#_FILENAME__auth')->getByUid($_G['uid']);
if ($auth==0) {
    $msg = "很抱歉,您没有权限访问此页面!";
    include template("_FILENAME_:Error");
	exit();
}

// 设置
$plugin_path = _FILENAME__env::get_plugin_path();
$filename = basename(__FILE__);
list($controller) = explode('.',$filename);
include template("_FILENAME_:".strtolower($controller));
_FILENAME__env::getlog()->trace("pv[".$_G['username']."|uid:".$_G['uid']."]");
C::t('#_FILENAME_#_FILENAME__log')->write("visit _FILENAME_:_FILENAME_");
