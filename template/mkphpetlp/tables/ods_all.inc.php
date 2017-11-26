<?php
/**
 * ODS表同步
 **/

// 统计日期
$statday = timeutil::getStatDay();

try {
    // 删除过期数据库
    mysql_util::drop_database_timeout('mysql_stat');

    // 创建目标数据库(如果不存在)
    $destdb = 'mysql_stat';
    mysql_util::create_database($destdb);

    // 要同步的table
    $wavedb = 'wave_mars';
    dump_ods_table($destdb,$wavedb,'g_city');

} catch (Exception $e) {
    $msg = $e->getMessage();
    ublog::fatal($msg);
    die("[fatal]\t$msg\n");
}

// ODS数据表同步到统计库
function dump_ods_table($destdb,$srcdb,$table,$where='')
{
    $begin = microtime();
    
    try {
        //1. 同步表结构
        $pdo = C::dbo($destdb); 
        $res = $pdo->queryAll("DESCRIBE $table");
        if (!empty($res)) {
            //ublog::debug("$destdb.$table 已存在");
        } else {
            $create_sql = mysql_util::get_create_table_sql($srcdb,$table);
            $pdo->exec($create_sql);
        }

        //2. 从ods表中导出数据到文件
        $datafile = fileutil::genFileName("ods.$srcdb.$table.xls");
        if (dis_file($datafile)) {
            throw new Exception("ods_table_has_dumped [$srcdb.$table]");
            return;     //!< 数据文件存在说明已经同步过,直接退出 
        }
        if (!dis_file($datafile)) {
            $sql = "SELECT * FROM $table";
            if ($where!='') $sql.=" WHERE $where";
            mysql_util::export_file($srcdb,$sql,$datafile);
        }

        //3. 将文件导入目标数据表
        mysql_util::import_file($datafile,$destdb,$table,$replace=false);
        throw new Exception("dump_ods_table_success [$srcdb.$table]");
    } catch(Exception $e) {
        $log = $e->getMessage();
        $end = microtime();

        list($begin_s,$begin_ms) = explode(' ',$begin);
        list($end_s,$end_ms) = explode(' ',$end);
        $tmdiff = ($end_s-$begin_s) + ($end_ms-$begin_ms);
        $tmdiff = number_format($tmdiff,3);
        ublog::trace($log." usetime: ".$tmdiff."s");
    }
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
