#!/bin/bash

# mvn打包
mvn clean install package -Dmaven.test.skip=true -U -e



##########################################
# 打包成可执行的环境
##########################################
product=com.companyname.app
outdir=output/$product
jar=$product.jar

if [ -d output ];then
    rm -rf output
fi

mkdir -p $outdir/properties

cp -r target/$jar $outdir/
cp -r src/main/resources/*.properties $outdir/properties/

# gen run.sh
cp -r run.sh $outdir/
sed -i "" "s/target\///g" $outdir/run.sh

# 压缩成zip包
cd output; zip -r $product.zip $product/*; cd ..

echo "!!!BUILD SUCCESS!!!"
