<?php

/***********************************************************
 * @file:   DAO.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2015-03-31 15:14:22
 * @modify: 2015-03-31 15:14:22
 * @brief:  所有DAO对象的基类
 ***********************************************************/

class DAO
{
	// 数据库访问对象
	protected $_pdo;
	// 数据表名
	protected $_table   = "";
	// 数据表字段
    protected $_columns = array();
	// 对象实例
	protected $_object  = array();

	public function __construct(&$pdo, $table, $columns)
	{
		$this->_pdo = &$pdo;

        $this->_table = $table;
        $this->_columns = $columns;

		$this->reset(); 
	}

	// 重置
	final public function reset() 
	{/*{{{*/
		$this->_object = array();
		foreach ($this->_columns as $column) {
			$k = $column[0];
			$v = end($column);
			$this->_object[$k] = $v;
		}
	}/*}}}*/

	final public function get($key) { return isset($this->_object[$key]) ? $this->_object[$key] : NULL; }
	final public function set($key, $value) { $this->_object[$key] = $value; }

	// 获取表名
	final public function getTable() { return $this->_table; }

	// 转成array数据类型
	final public function toArray() { return $this->_object; }
	// 转成JSON串
	final public function toJSON() { return json_encode($this->_object); }


	protected function getColumns()
	{/*{{{*/
        $columns = "";
        $sp = "";
        foreach ($this->_columns as $column) {
            $columns .= $sp.'`'.$column[0].'`';
            $sp = ",";
        }
		return $columns;
	}/*}}}*/

	// 从数据库中查询加载
    final public function load(array $conds)
    {/*{{{*/
        //1. generate sql
        $table = $this->_table;
		$columns = $this->getColumns();
        $condition = "1";
        foreach ($conds as $cond) {
            $condition .= " and $cond";
        }   
        $sql = "SELECT $columns FROM $table WHERE $condition";
		if (Env::isDebug()) {
			Env::getlog("sql")->trace($sql);
		}
       
        //2. query
        $smt = $this->_pdo->query($sql);
        if ($smt && $row = $smt->fetch()) {
             array_unset_int_key($row);
             $this->_object = $row;
			 $smt = NULL;
             return true;
        } else {
			 $smt = NULL;
             return false; 
        }   
    }/*}}}*/

	// 从array中加载
	final public function loadFromArray(array $arr)
	{/*{{{*/
		$this->reset();
        foreach ($this->_columns as $column) {
			$k = $column[0];
			isset($arr[$k]) && $this->_object[$k] = $arr[$k];
        }
	}/*}}}*/

	// 从数据库中查询
	final public function query(array $conds, $start=0, $limit=30, $sort=NULL, $dir=NULL)
	{/*{{{*/
		$pdo = &$this->_pdo;
		$table = $this->_table;
		$condition = "1";
        foreach ($conds as $cond) {
            $condition .= " and $cond";
        }   
		$res = array (
			"totalProperty" => 0,
			"root" => array(),
		);
		
		//1. query totalProperty
		$sql = "SELECT count(*) as num FROM $table WHERE $condition";
        $smt = $pdo->query($sql);
        if ($smt && $row = $smt->fetch()) {
			$res["totalProperty"] = intval($row["num"]);
        }   
		$smt = NULL;
		if (Env::isDebug()) {
			Env::getlog("sql")->trace($sql);
		}
		
		//2. query current page
		if ($res["totalProperty"] == 0) {
			return $res;
		}
		$columns = $this->getColumns();
		$limit_statement = "";
		if ($limit > 0) {
			$limit_statement = "LIMIT $start, $limit";
		}
		$sort_statement = "";
		if ($sort!==NULL && $dir!=NULL) {
			$sort_statement = "ORDER BY $sort $dir";
		}
		$sql = "SELECT $columns FROM $table WHERE $condition $sort_statement $limit_statement";
		$smt = $pdo->query($sql);
		while ($smt && $row=$smt->fetch()) {
			array_unset_int_key($row);
			$res["root"][] = $row;
		}
		$smt = NULL;
		if (Env::isDebug()) {
			Env::getlog("sql")->trace($sql);
		}

		return $res;
	}/*}}}*/

	// 查询主键对应的记录map
	final public function map($pkey, array &$idarr, array &$map, array $conds=array())
	{/*{{{*/
		$total = count($idarr); 
		if ($total == 0) {
			return;
		}
		$buf = array();
		$start = 0;
		$limit = 30;
		while ($start < $total) { 
			$buf = array_slice($idarr, $start, $limit);
			$this->_map($pkey, $buf, $map, $conds);
			$start += $limit; 
		}
	}/*}}}*/
	private function _map($pkey, array &$idarr, array &$map, array $conds=array())
	{/*{{{*/
		if (count($idarr) == 0) {
			return;
		}
		$ids = implode(",", $idarr);
		$columns = $this->getColumns();
		$table = $this->_table;
		$condition = "$pkey in ($ids)";
		if (count($conds)>0) {
			foreach ($conds as $cond)
				$condition .= " and $cond";
		}
	
		$sql = "SELECT $columns FROM $table WHERE $condition";
		if (Env::isDebug()) {
			Env::getlog("sql")->trace($sql);
		}
		$smt = $this->_pdo->query($sql);
		while ($smt && $row=$smt->fetch()) {
			array_unset_int_key($row);
			$k = $row["$pkey"];
			if (!isset($map[$k]))
				$map[$k] = $row;
		}
	}/*}}}*/

    // 统计个数
    final public function queryCount(array $conds)
    {/*{{{*/
		$pdo = &$this->_pdo;
		$table = $this->_table;
		$condition = "1";
        foreach ($conds as $cond) {
            $condition .= " and $cond";
        }   
		$res = 0;
		$sql = "SELECT count(*) as num FROM $table WHERE $condition";
        $smt = $pdo->query($sql);
        if ($smt && $row = $smt->fetch()) {
			$res = intval($row["num"]);
        }   
		$smt = NULL;
        return $res;
    }/*}}}*/

    // 持久化对象
	public function save() 
	{/*{{{*/
		$pdo = $this->_pdo;
		$table = $this->_table;

		$columns = "";
		$values = "";
		$upcols = "";
		$sp = "";
		foreach ($this->_object as $column => $value) {
			$v = real_escape_string($value);
			$values.= $sp."'$v'";
			$columns .= $sp.$column;
			$upcols .= $sp."$column=values($column)";
			$sp = ",";
		}
		$sql = "INSERT INTO $table ($columns) VALUES ($values) ON DUPLICATE KEY UPDATE $upcols";
		return $pdo->exec($sql);
	}/*}}}*/

	// 更新部分字段
	public function update(array &$kvmap, array $conds)
	{/*{{{*/
		if (count($kvmap) == 0 || count($conds) == 0) {
			return;
		}

		$pdo = $this->_pdo;
		$table = $this->_table;
		
		$upsmt = "";
		$sp = "";
		foreach ($this->_columns as $item) {
			$column = $item[0];
			if (isset($kvmap[$column])) {
				$value = real_escape_string($kvmap[$column]);
				$upsmt.= $sp."$column = '$value'";
				$sp = ",";
			}
		}
        $condition = "1";
        foreach ($conds as $cond) {
            $condition .= " and $cond";
        }   
		$sql = "UPDATE $table SET $upsmt WHERE $condition";
		return $pdo->exec($sql);
	}/*}}}*/

}

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
