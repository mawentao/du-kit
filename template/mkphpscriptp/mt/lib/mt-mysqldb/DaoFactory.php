<?php

/***********************************************************
 * @file:   DaoFactory.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2015-06-28 08:52:35
 * @modify: 2015-06-28 08:52:35
 * @brief:  Dao����
 ***********************************************************/

class DaoFactory
{
    // DBO�����أ�ÿ�����ݿ�ֻ��һ��dboʵ��
    private static $_dbo_pool = array();

    // DAO�����أ�ÿ�ű�ֻ��һ��daoʵ��
    private static $_dao_pool = array();

    // DB����
    private static $_db_conf = array();

    // �������
    public static function addconf($dbconf_xml)
    {/*{{{*/
        $xml = simplexml_load_file($dbconf_xml, null);
        foreach ($xml->db as $db) {
            $attrmap = $db->attributes();
            $dbname = trim($attrmap["name"]);
            self::$_db_conf[$dbname] = array (
                "host" => trim($attrmap["host"]),
                "port" => trim($attrmap["port"]),
                "user" => trim($attrmap["user"]),
                "pass" => trim($attrmap["password"]),
                "tables" => array()
            );
            $tables = &self::$_db_conf[$dbname]["tables"];
            foreach ($db->table as $table) {
                $tableAttrs = $table->attributes();
                $tableName  = trim($tableAttrs["name"]);
                $tables[$tableName] = array();
                foreach ($table->column as $column) {
                    $columnAttrs = $column->attributes();
                    $tables[$tableName][] = array (
                        "name" => trim($columnAttrs["name"]),
                        "defaultValue" => trim($columnAttrs["defaultValue"]),
                    );
                }
            }
        }
    }/*}}}*/

    // ��ȡDBO
    public static function getDBO($dbname)
    {/*{{{*/
        if (!isset(self::$_dbo_pool[$dbname])) {
            $dbconf = &self::$_db_conf[$dbname];
            //print_r($dbconf);
            $host = $dbconf["host"];
            $port = $dbconf["port"];
            $user = $dbconf["user"];
            $pass = $dbconf["pass"];
            self::$_dbo_pool[$dbname] = new DBO($host, $port, $user, $pass, $dbname);
        }
        return self::$_dbo_pool[$dbname];
    }/*}}}*/
    
    // ��ȡtableDao
    public static function getDAO($dbname, $tableName)
    {/*{{{*/
        $key = $dbname."_".$tableName;
        if (!isset(self::$_dao_pool[$key])) {
            $columns = array();
            foreach (self::$_db_conf[$dbname]["tables"][$tableName] as &$item) {
                $columns[] = array($item["name"], $item["defaultValue"]);
            }
            $pdo = self::getDBO($dbname);
            self::$_dao_pool[$key] = new DAO($pdo, $tableName, $columns);
        }
        return self::$_dao_pool[$key];
    }/*}}}*/
}

if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME']))
{

     DaoFactory::init("/home/work/local/apache2/htdocs/mt/test/conf/mysqldb.xml");
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
