# project-setenv-resolution
作为检测c代码基础的小项目：How to change resolution by modifing uboot-env

添加工程文件:
boot.ini为系统使用的uboot环境，
因为之前App搞过一个apk来实现这个功能，目前只需要做到如下
a.out 1920x1080 之行后会将boot.ini内的相应的字符串替换掉就是完成功能。

在Android下使用Key Map，在 c 语言里面呢 ?
注：rubbish.c是一个垃圾文件，仅仅是假想的构思一下，正式请使用resolution.c

-------------------------------------------------
参考文档
C语言实现修改文本文件中特定行的实现代码
http://www.jb51.net/article/39040.htm

================================================
思考
1. 关于最开始的数据结构可能设计的有问题，明明是同类型的字符串两个，偏偏设计到一个结构体里面，不过这样也方便了很多。。

2. 关于内存的申请与释放， 需要把握好哪些方面呢，防止下次重复的犯错误。


####buglist######
oh no, 执行
./a.out -d lcd -r 1024x768
行尾居然出现fbmem=28M,28MM
多了一个大写的 M 
