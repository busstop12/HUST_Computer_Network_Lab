#!/bin/bash
# appname 程序名称
# inputname 输入文件名
# outputname 输出文件名
# resultname 程序控制台输出结果重定向文件名

echo "/********************************************************/"
appname='StopWait'
inputname='input.txt'
outputname='StopWait_output.txt'
resultname='StopWait_result.txt'

for ((i=1;i<=10;i++))
do
echo Test $appname $i
./$appname > $resultname 2>&1
cmp $inputname $outputname
echo Test $i over
done

echo "/********************************************************/"
appname='GBN'
inputname='input.txt'
outputname='GBN_output.txt'
resultname='GBN_result.txt'

for ((i=1;i<=10;i++))
do
echo Test $appname $i
./$appname > $resultname 2>&1
cmp $inputname $outputname
echo Test $i over
done

echo "/********************************************************/"
appname='SR'
inputname='input.txt'
outputname='SR_output.txt'
resultname='SR_result.txt'

for ((i=1;i<=10;i++))
do
echo Test $appname $i
./$appname > $resultname 2>&1
cmp $inputname $outputname
echo Test $i over
done

echo "/********************************************************/"
appname='TCP'
inputname='input.txt'
outputname='TCP_output.txt'
resultname='TCP_result.txt'

for ((i=1;i<=10;i++))
do
echo Test $appname $i
./$appname > $resultname 2>&1
cmp $inputname $outputname
echo Test $i over
done
