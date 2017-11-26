#!/usr/bin/env php
<?php
/***********************************************************
 * @file:   _FILENAME_.php
 * @author: _AUTHOR_
 * @create: _DATETIME_
 * @modify: _DATETIME_
 * @brief:  ETL Script Entrance
 ***********************************************************/

ini_set('date.timezone','Asia/Shanghai');
define('SCRIPT_PATH',dirname(__FILE__));
error_reporting(E_ALL);

require_once SCRIPT_PATH.'/function/function_core.php';

if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME']))
{
    if (!isset($argv[1]) || $argv[1]=="") {
        $exe = $argv[0];
        echo "[usage]: php $exe SCRIPT_NAME [date] [-r]\n";
        exit(0);
    }
    C::init();

    $lastarg = end($argv);
    if ($lastarg=='-r') { rollback::$is_rollback=true; }
    $arg = $argv[1];
    $scriptFile = SCRIPT_PATH."/".$arg;
    if (!is_file($scriptFile)) {
        $scriptFile = SCRIPT_PATH."/tables/$arg.inc.php";
    }
    if (!is_file($scriptFile)) {
        die("file not exist: [$scriptFile]\n");
    }
    ublog::trace("begin_run $scriptFile");
    include_once($scriptFile);
    ublog::trace("end_run $scriptFile");
    die("[success]\n");
}

?>
