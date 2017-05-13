# project-setenv-resolution
作为检测c代码基础的小项目：How to change resolution by modifing uboot-env

添加工程文件:
boot.ini为系统使用的uboot环境，
因为之前App搞过一个apk来实现这个功能，目前只需要做到如下
a.out 1920x1080 之行后会将boot.ini内的相应的字符串替换掉就是完成功能。

在Android下使用Key Map，在 c 语言里面呢 ?
注：rubbish.c是一个垃圾文件，仅仅是假想的构思一下，正式请使用resolution.c
