<?php
/***********************************************************
 * @file:   Test.php
 * @author: _AUTHOR_
 * @create: _DATETIME_
 * @modify: _DATETIME_
 * @brief:  Test.php
 ***********************************************************/
class Test extends BaseScript
{
    // 执行脚本
    protected function execute($args)
    {
        print_r($args);
        print "!!!THE END!!!\n";
    }
}
// vim600: sw=4 ts=4 fdm=marker syn=php
?>
