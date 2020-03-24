# Wonderful操作系统
目前新版已经做好了，周末会上传新版代码，注意，新版本目录结构发生改变，因此本版本将保存为v2分支中。

#### 介绍
![](https://gitee.com/kkstudio2019/wonderful/raw/master/yulan.png)  
一款由RDPStudio开发的操作系统（我们拥有完整版权）。目前已支持中文。  
本项目不是低创项目，下一版本会增加背景等功能。  
目前它不可以作为您的日常操作系统，不过V5.0将可以作为日常系统。  
我们没有抄袭，只是借鉴了贴吧前者（[贴吧地址](https://tieba.baidu.com/p/6533511510)[他们的官网](https://www.panj.tk/)）的配色、背景和底部设计。  
上图中，可以看出，程序比贴吧前者的多，因此绝对不被抄袭。  
日文正常现象，不要大惊小怪，和点X。

#### 待实现功能(仅下一版本)
Office套件(Excel、PPT、Word)  
媒体播放器  
网络

#### 软件架构
仅供X86设备使用。


#### 开发教程及相关指令

1. 开发教程<br/><br/>
环境准备：<br/>
先下载源码，放在一个文件夹里，如`D:\WonderfulOS\src`中，然后在上级目录`D:\WonderfulOS`中运行[开发工具包](https://gitee.com/kkstudio2019/wonderful/releases/v1.0)自动解压器，即可完成环境准备<br/><br/>
正式开始：<br/>
先删掉haribote.img，这是编译输出，再执行make full完成编译部分，之后make run即可虚拟机启动。程序可以参考各个目录中的c程序（注意！程序不要忘了单独make！）。如需添加hrb程序，在makefile里的haribote.img节点里添加copy指令复制hrb程序即可。

2.  指令部分<br/>
`D:\WonderfulOS\src>make full #完整编译`<br/>
`D:\WonderfulOS\src>make clean_full #完整清理`<br/>
`D:\WonderfulOS\src>make run #自动编译+运行（必须先执行完整编译）`<br/>
其他指令详见Makefile

3. [开发工具包](https://gitee.com/kkstudio2019/wonderful/releases/v1.0)，选我最方便！

#### 参与贡献

1.  下载 本仓库
2.  Fork 本仓库
2.  打包上传代码
4.  新建 Feat_xxx 分支
5.  提交代码
6.  新建 Pull Request
