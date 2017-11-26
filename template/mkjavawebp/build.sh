#!/bin/bash

# mvn打包
mvn clean install package -Dmaven.test.skip=true -U -e

