# Wonderful操作系统

#### 介绍
![](https://gitee.com/kkstudio2019/wonderful/raw/master/yulan.png)  
一款由RDPStudio开发的操作系统（我们拥有完整版权）。

#### 软件架构
仅供X86设备使用。


#### 开发教程及相关指令

1. 开发教程<br/><br/>
环境准备：<br/>
先下载源码，放在一个文件夹里，如`D:\WonderfulOS\src`中，然后在上级目录`D:\WonderfulOS`中运行[开发工具包](https://gitee.com/kkstudio2019/Wonderful/releases/v1.0)自动解压器，即可完成环境准备<br/><br/>
正式开始：<br/>
先执行make full完成编译部分，之后make run即可虚拟机启动。程序可以参考各个目录中的c程序（注意！程序不要忘了单独make！）。如需添加hrb程序，在makefile里的haribote.img节点里添加copy指令复制hrb程序即可。

2.  指令部分<br/>
`D:\WonderfulOS\src>make full #完整编译`<br/>
`D:\WonderfulOS\src>make clean_full #完整清理`<br/>
`D:\WonderfulOS\src>make run #自动编译+运行（必须先执行完整编译）`<br/>
其他指令详见Makefile

3. [开发工具包](https://gitee.com/kkstudio2019/Wonderful/releases/v1.0)，选我最方便！

#### 参与贡献

1.  下载 本仓库
2.  Fork 本仓库
2.  打包上传代码
4.  新建 Feat_xxx 分支
5.  提交代码
6.  新建 Pull Request
