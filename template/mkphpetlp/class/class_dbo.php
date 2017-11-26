<?php

/***********************************************************
 * @file:   DBO.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2014-05-07 14:41:32
 * @modify: 2014-05-07 14:41:32
 *          2014-07-17 14:24:48 �����Ƿ��ӡsql��־ѡ��
 * @brief:  ����Mysql���ݿ�ļ򵥷�װ
 ***********************************************************/

class DBO
{
	private $pdo = null;

	/* ���캯�� */
	public function __construct($host, $port, $user, $pass, $db)
	{
		$this->pdo = new PDO("mysql:host=$host;port=$port;dbname=$db", "$user", "$pass",
                             array(PDO::ATTR_PERSISTENT=>false));
		//$this->pdo->query("set names UTF8", false);
	}

	/**
	 * PDO ԭ���ӿڼ��˼�ʱ��װ
	 * @param sql
	 * @param log
	 * @return exec_res
	 **/
	public function exec($sql, $log=true)
	{/*{{{*/
		$stime = $this->getmicrotime();
		$res = $this->pdo->exec($sql);
		$etime = $this->getmicrotime();
		if ($log)
			$this->logsql($sql, $stime, $etime);
		return $res;
	}/*}}}*/

	/**
	 * PDOԭ���ӿڼ��˼�ʱ��װ
	 * @param sql
	 * @param log
	 * @return smt
	 **/
	public function query($sql, $log=false) 
	{/*{{{*/
		$stime = $this->getmicrotime();
		$smt = $this->pdo->query($sql);
		$etime = $this->getmicrotime();
		if ($log)
			$this->logsql($sql, $stime, $etime);
		return $smt;
	}/*}}}*/

	public function beginTransaction() { $this->pdo->beginTransaction(); }
	public function commit() { $this->pdo->commit(); }
	public function rollBack() { $this->pdo->rollBack(); }


	/////////////////////////////////////////////////////////////////////////////////////
	// �������Զ����һЩ�ӿ�

	/* ���¼�¼ */
	public function update($table, array $columns, array $values, array $conditions)
	{/*{{{*/
		$sql = "UPDATE $table SET ";
		$sp  = "";
		$len1 = count($columns);
		$len2 = count($values);
		$len = $len1<$len2 ? $len1 : $len2;
		for ($i=0; $i<$len; ++$i) {
			$col = $columns[$i];
			$val = $values[$i];
			$v = mysql_real_escape_string($val);
			$sql.= $sp."$col='$v'";
			$sp = ",";
		}
		if (count($conditions) > 0) {
			$sp = " WHERE ";
			foreach ($conditions as $con) {
				$sql.= $sp.$con;
				$sp = " AND ";
			}
		}
		return $this->exec($sql);
	}/*}}}*/
	
	/* ���浥����¼ */
	public function save($table, array $columns, array $values, array $upcols=array())
	{/*{{{*/
		$sql = "INSERT INTO $table (";
		$sp = "";
		foreach ($columns as $col) {
			$sql.= $sp."$col";
			$sp = ",";
		}
		$sql.=") VALUES (";
		$sp = "";
		foreach ($values as &$value) {
			$v = mysql_real_escape_string($value);
			$sql.= $sp."'$v'";
			$sp = ",";
		}
		$sql.=")";

		if (count($upcols)>0) {
			$sql.=" ON DUPLICATE KEY UPDATE ";
			$sp = "";
			foreach ($upcols as $col) {
				$sql.= $sp."$col=values($col)";
				$sp = ",";
			}
		}
		return $this->exec($sql);
	}/*}}}*/

	/* �������� */
	public function savebat($table, array $columns, array &$rows, array $upcols=array())
	{/*{{{*/
		$sql = "INSERT INTO $table (";
		$sp = "";
		foreach ($columns as $col) {
			$sql.= $sp."$col";
			$sp = ",";
		}
		$sql.=") VALUES ";
		$sp = "";
		foreach ($rows as &$values) {
			$sql.= $sp."(";
			$sp2 = "";
			foreach ($values as &$value) {
				$v = mysql_real_escape_string($value); //str_replace("'", "\\'", $value);
				$sql.= $sp2."'$v'";
				$sp2 = ",";
			}
			$sql.=")";
			$sp = ",";
		}

		if (count($upcols)>0) {
			$sql.=" ON DUPLICATE KEY UPDATE ";
			$sp = "";
			foreach ($upcols as $col) {
				$sql.= $sp."$col=values($col)";
				$sp = ",";
			}
		}
		return $this->exec($sql);
	}/*}}}*/

	/**
	 * ��ѯȫ�������
	 * @param sql: sql���
	 * @param log: �Ƿ������־
	 * @return array
	 **/
	public function queryAll($sql, $log=false)
	{/*{{{*/
		$smt = $this->query($sql, $log);
		if ( !$smt ) {
			return array(); 
		} else {
            $arr = array();
            while ($row=$smt->fetch()) {
                // ȥ�������±�
                foreach ($row as $key => &$value) {
                    if (is_int($key)) { unset($row[$key]); }
                }
                $arr[] = $row;
            }
			$smt = null;
			return $arr;
		}
	}/*}}}*/

	/**
	 * ��ѯ����
	 * @param table: ��ѯ�ı���
	 * @param conditions: ��ѯ����
	 * @param log: �Ƿ������־
	 * @return int
	 **/
	public function queryCount($table, array $conditions=array(), $log=false)
	{/*{{{*/
		$sql = "SELECT COUNT(*) as num FROM $table";
		if (count($conditions) > 0) {
			$sp = " WHERE ";
			foreach ($conditions as $con) {
				$sql.= $sp.$con;
				$sp = " AND ";
			}
		}
		$arr = $this->queryAll($sql, $log);
		if (count($arr)>0) {
			return $arr[0]["num"];
		} else {
			return 0;
		}
	}/*}}}*/

	public function lastInsertId() { return $this->pdo->lastInsertId(); }

	/////////////////////////////////////////////////////////////////

	private function getmicrotime()
	{
		list($usec, $sec) = explode(" ",microtime());
		return ((float)$usec + (float)$sec);
	}
	private function logsql($sql, $stime, $etime)
	{
		$timespan = ($etime-$stime)*1000;

		if ($timespan < 1000) {
			$timespan = number_format($timespan, 3). " ms";
		} else {
			$timespan /= 1000;
			$timespan = number_format($timespan, 3). " s";
		}
		//echo "sql:[$sql], time:[$timespan]\n";
	}
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
