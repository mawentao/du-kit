#!/bin/bash
####################################################
# @file:   build.sh
# @author: _AUTHOR_
# @create: _DATETIME_
# @modify: _DATETIME_
# @brief:  build.sh
####################################################

pluginname="_FILENAME_"
pluginversion="1.0"
outdir="output/$pluginname"
buildtime=`date +%Y%m%d%H%M%S`
tarname="$pluginname-$pluginversion-$buildtime.zip"
src="dist-$buildtime"

function cpfiles()
{
    for i in $@; do
        cp -r $i $outdir
    done
}

################################
rm -rf output
mkdir -p $outdir
################################
# 压缩混淆js代码
node r.js -o build.js
rm -rf template/dist/build.txt
rm -rf template/dist/er
################################
cpfiles api conf *.php *.xml class table model template cron
################################
rm -rf $outdir/template/src
mv $outdir/template/dist $outdir/template/$src
sed -i "" "s/src\//$src\//g" $outdir/template/header.htm
sed -i "" "s/mwt3.2utf8 (http:\/\/10.3.70.15:8008\/discuz\/)/dz3.2utf8 (http:\/\/192.168.0.1\/dz)/g" $outdir/discuz_plugin__FILENAME_.xml
sed -i "" "s/X3.2/X2.5,X3,X3.1,X3.2/g" $outdir/discuz_plugin__FILENAME_.xml
################################
cd $outdir
# 删除php文件中的所有注释代码
../../clear_annotation -r -w
#iconv -f UTF-8 -t GBK discuz_plugin__FILENAME_.xml > discuz_plugin__FILENAME__SC_GBK.xml
mv discuz_plugin__FILENAME_.xml discuz_plugin__FILENAME__SC_UTF8.xml
find . -type d -name ".svn" | xargs rm -rf
find . -name "*.bk" | xargs rm -rf
cd ../; zip -r $tarname $pluginname
cd ../
################################

echo 'build success'
exit 0
