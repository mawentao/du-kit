#!/bin/sh

# 统计前一天数据
statday=`date +"%Y%m%d" -d "-1 days"`

function run()
{
    php run.php $1 $statday
}

tables=(
    "tables/ods_all.inc.php"            # ODS数据源同步
    "tables/log_nginx_access.inc.php"   # nginx访问日志切割
)

for v in ${rollback_tables[@]}; do
    run $v 
done

