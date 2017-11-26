<?php

/* 去除数组中的数字下标 */
function array_unset_int_key(array &$arr)
{/*{{{*/
	foreach ($arr as $key => &$value) {
		if (is_int($key)) { unset($arr[$key]); }
	}
}/*}}}*/

/**
 * mysql_real_escape_string的自定义版本
 * 此版本无需事先连接数据库
 **/
function real_escape_string($str)
{/*{{{*/
    if (!is_string($str)) return $str;
    $len = strlen($str);
    if ($len==0) return $str;
    $res = "";
    for ($i=0; $i<$len; ++$i) {
        $c = $str[$i];
        if ($c=="\r") $c = "\\r";
        if ($c=="\n") $c = "\\n";
        if ($c=="\x00") $c = "\\0";
        if ($c=="\x1a") $c = "\\Z";
        if ($c=="'" || $c=='"' || $c=='\\') $res.="\\";
        $res.= $c; 
    }
    return $res;
}/*}}}*/


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
