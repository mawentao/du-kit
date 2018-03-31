<?php
if(!defined('IN_DISCUZ')) {
    exit('Access Denied');
}
/**
 * _FILENAME_
 * @author: _AUTHOR_
 * @create: _DATETIME_
 **/
class _FILENAME_ extends discuz_table
{
    public function __construct() {
        $this->_table = '_FILENAME_';
        $this->_pk = 'rid';
        parent::__construct();
    }

    // 根据主键获取记录
    public function get_by_pk($id) {
        $sql = "SELECT * FROM ".DB::table($this->_table)." WHERE ".$this->_pk."='$id'";
        return DB::fetch_first($sql);
    }

    // 查询接口
    public function query()
    {/*{{{*/
        $return = array(
            "totalProperty" => 0,
            "root" => array(),
        );
        $key   = demoapp_validate::getNCParameter('key','key','string');
        $sort  = demoapp_validate::getOPParameter('sort','sort','string',1024,'ctime');
        $dir   = demoapp_validate::getOPParameter('dir','dir','string',1024,'DESC');
        $start = demoapp_validate::getOPParameter('start','start','integer',1024,0);
        $limit = demoapp_validate::getOPParameter('limit','limit','integer',1024,20);
        $where = "isdel=0";
        if ($key!="") $where.=" AND (name like '%$key%' OR tel like '%$key%')";
        $table = DB::table($this->_table);
        $sql = <<<EOF
SELECT SQL_CALC_FOUND_ROWS *
FROM $table
WHERE $where
ORDER BY $sort $dir
LIMIT $start,$limit
EOF;
        $return["root"] = DB::fetch_all($sql);
        $row = DB::fetch_first("SELECT FOUND_ROWS() AS total");
        $return["totalProperty"] = $row["total"];
        return $return;
    }/*}}}*/

    // 保存记录
    public function save()
    {/*{{{*/
        $rid = demoapp_validate::getNCParameter('rid','rid','integer');
        $record = array (
            'name' => demoapp_validate::getNCParameter('name','name','string',32),
            'tel' => demoapp_validate::getNCParameter('tel','tel','string',32),
        );
        if ($rid==0) {
            $record['ctime'] = date('Y-m-d H:i:s');
            return $this->insert($record);
        } else {
            return $this->update($rid,$record);
        }
    }/*}}}*/

    // 删除记录
    public function remove()
    {
        $rid = demoapp_validate::getNCParameter('rid','rid','integer');
        return $this->update($rid,array('isdel'=>1));
    }

}

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
