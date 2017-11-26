<?php
/**
 *    nginx access log 切割处理
 **/

$statday = timeutil::getStatDay();

// nginx log目录
$nginx_logs_path = '/Users/mawentao/local/nginx/logs';
$nginx_access_log_file = "$nginx_logs_path/localhost.access.log";
$logfile_statday = fileutil::genFileName("nginx.access.$statday.log");
$tablefile = fileutil::genFileName("nginx.access.$statday.xls");

try {
    //1. 切割文件
    if (!dis_file($logfile_statday)) {
        // 移动文件
        $cmd = "mv $nginx_access_log_file $logfile_statday";
        runcmd($cmd);
        // 向 Nginx 主进程发送 USR1 信号。USR1 信号是重新打开日志文件
        $cmd = "kill -USR1 $(cat $nginx_logs_path/nginx.pid)";
        runcmd($cmd);
    }
    //2. 处理文件
    proc_nginx_access_log_file($logfile_statday,$tablefile);
    //3. 
    echo ">>> nginx_access_log_file: $logfile_statday\n";
    echo ">>> table_file: $tablefile\n";
} catch (Exception $e) {
    $msg = $e->getMessage();
    ublog::warning($msg);
}


function proc_nginx_access_log_file($infile,$outfile)
{
    //1. 打开输入输出文件
    $fi = fopen($infile,  "r");
    $fo = fopen($outfile, "w");
    if (!$fi) throw new Exception("open file fail: $infile");
    if (!$fo) throw new Exception("open file fail: $outfile");
    //2. 处理文件
    while(!feof($fi))
    {   
        $line = trim(fgets($fi));
        if($line == ""){continue;}
        $outstr = proc_line($line);        
        fprintf($fo, "%s\n", $outstr);
    }
    //3. 关闭文件
    fclose($fi);
    fclose($fo);
}

function proc_line($line)
{
    return $line;
}

?>
