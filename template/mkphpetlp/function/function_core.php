<?php

require_once libfile('class/core');


// copy from discuzx3.2
function libfile($libname, $folder = '') 
{
    $libpath = dirname(dirname(__FILE__)).'/'.$folder;
    if(strstr($libname, '/')) {
        list($pre, $name) = explode('/', $libname);
        $path = "{$libpath}/{$pre}/{$pre}_{$name}";
    } else {
        $path = "{$libpath}/{$libname}";
    }
    return $path.".php";
}

// 判断文件是否存在(带刷新)
function dis_file($f)
{
    clearstatcache();
    return is_file($f); 
}

// 使用system执行cmd
function runcmd($cmd)
{
    ublog::trace("runcmd: $cmd");
    system($cmd);
}


// 计算两个点的距离 
function lang_distance($lat1,$lng1,$lat2,$lng2)
{
    $d1 = floatval($lat2) - floatval($lat1);
    $d2 = floatval($lng2) - floatval($lng1);
    $d = sqrt($d1*$d1 + $d2*$d2);
    //echo "($lat1,$lng1) - ($lat2,$lng2) = $d\n";
    return $d;
}

// 数组转成insert value
function arr2insertvalue(array $im)
{
    $arr = array();
    foreach ($im as $k => $v) {
        if (is_int($k)) continue;
        $arr[] = "'$v'";
    }
    return implode(",",$arr);
}

?>
