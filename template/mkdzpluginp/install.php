<?php
/*******************************************************
 * 此脚本文件用于插件的安装
 * 提示：可使用runquery() 函数执行SQL语句
 *       表名可以直接写“cdb_”
 * 注意：需在导出的 XML 文件结尾加上此脚本的文件名
 *******************************************************/
if(!defined('IN_DISCUZ')) {
	exit('Access Denied');
}

$addtime = $modtime = date('Y-m-d H:i:s');

// 用户权限表
$table = DB::table('_FILENAME__auth');
/*{{{*/
$sql = "CREATE TABLE IF NOT EXISTS $table ". <<<EOF
(
`uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'DZ用户ID',
`auth` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '权限(0:无权限,1:普通用户,2:高级用户)',
`ctime` datetime NOT NULL DEFAULT "0000-00-00 00:00:00" comment '创建日期',
`mtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
PRIMARY KEY (`uid`)
) ENGINE=MyISAM COMMENT '用户权限表'
EOF;
runquery($sql);
$sql="INSERT IGNORE INTO $table (uid,auth,ctime) VALUES (1,2,'$addtime')";
runquery($sql);
/*}}}*/

// 用户日志
/*{{{*/
$table = DB::table('_FILENAME__log');
$sql = "CREATE TABLE IF NOT EXISTS $table ". <<<EOF
(
`logid` bigint unsigned NOT NULL AUTO_INCREMENT COMMENT '日志ID(自增主键)', 
`logtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '日志时间',
`uid` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT '用户ID',
`client_ip` varchar(32) NOT NULL DEFAULT '' COMMENT '来访IP',
`log_content` varchar(4096) NOT NULL DEFAULT '' COMMENT '日志内容',
PRIMARY KEY (`logid`),
KEY `idx_logtime_uid` (`logtime`,`uid`)
) ENGINE=InnoDB
EOF;
runquery($sql);
runquery("ALTER TABLE `$table` ENGINE=INNODB");
/*}}}*/

$finish = TRUE;
?>
