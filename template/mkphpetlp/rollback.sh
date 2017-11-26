#!/bin/sh

datebeg=20171119
dateend=20171122
beg_s=`date -d "$datebeg" +%s`
end_s=`date -d "$dateend" +%s`

rollback_tables=(
    "ods_all"
)

function rollback()
{
    php run.php $1 $2 -r
}

while [ "$beg_s" -le "$end_s" ]
do
    day=`date -d @$beg_s +"%Y%m%d"`

    echo "================ rollback [ $day ] ================"

    for v in ${rollback_tables[@]}; do
        rollback $v $day
    done

    beg_s=$((beg_s+86400))
done
