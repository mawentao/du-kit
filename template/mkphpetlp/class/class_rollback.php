<?php
/**
 * 回滚模块
 **/
class rollback
{
    private static $_has_rollbacked=array();
    public static $is_rollback=false;

    // 回滚stat表
    public static function rollback_stat($pdo,$table,$files)
    {
        if (!self::$is_rollback) return;
        // 已经回滚过的不再回滚
        if (isset(self::$_has_rollbacked[$table])) return;
        // 回滚
        ublog::trace("rollback $table");
        $pdo->exec("DROP TABLE $table");    //!< 删除数据表
        // 删除数据文件
        if (is_array($files)) {
            foreach ($files as $f) {
                if (dis_file($f)) {
                    unlink($f);
                }   
            }   
        } else {
            if (dis_file($files)) {
                unlink($files);
            }   
        } 
        // 
        self::$_has_rollbacked[$table] = true;  
    }

    // 回滚report表
    public static function rollback_report($pdo,$sql,$files)
    {
        if (!self::$is_rollback) return;
        // 已经回滚过的不再回滚
        if (isset(self::$_has_rollbacked[$sql])) return;
        // 回滚
        ublog::trace("rollback $sql");
        if ($sql!='') {
            $pdo->exec($sql);
        }
        // 删除数据文件
        if (is_array($files)) {
            foreach ($files as $f) {
                if (dis_file($f)) {
                    unlink($f);
                }   
            }   
        } else {
            if (dis_file($files)) {
                unlink($files);
            }   
        } 
        // 
        self::$_has_rollbacked[$sql] = true;  
    }
}
// vim600: sw=4 ts=4 fdm=marker syn=php
?>
