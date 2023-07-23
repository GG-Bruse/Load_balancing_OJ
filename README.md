## 安装环境要求

轻量应用服务器(需开放8000-10000号端口)，CentOS7操作系统，gcc版本因高于7号版本(9号版本最宜)，MySQL数据库

 

//安装scl 

sudo yum install centos-release-scl scl-utils-build 

//安装新版本gcc 

sudo yum install -y devtoolset-9-gcc devtoolset-9-gcc-c++

//启动

scl enable devtoolset-9 bash 

//打开配置文件，将启动命令写入最后一行，即可永久性启动

vim ~/.bash_profile 

 

## 安装、启动过程

### 1. 安装、启动主服务器

（1）解压安装包 

tar -zxvf release.tar.gz

（2）安装jsoncpp库

sudo yum install -y jsoncpp-devel

（3）安装ctemplate库

进入 ~/release/makeuse/ctemplate目录，./autogen.sh ，./configure ，make ，sudo make install ，sudo ln -s /usr/local/lib/libctemplate.so.3 /lib64/libctemplate.so.3先后执行上述五个命令

（4）安装MySQL C connect库

进入 ~/release/makeuse/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64/lib目录。使用pwd命令查看当前路径。

sudo ln -s {pwd命令结果}/libmysqlclient.so.18 /lib64/libmysqlclient.so.18

（5）在MySQL中设计与第三章中一致即可（olinejudge库，user、administrators、questionlist、joblist表）。并将MySQL用户名与密码替换入~/release/MainServer/conf/mysqluser.conf配置文件中

（6）将准备部署代码处理服务器的机器的IP地址与准备绑定的端口号，写入~/release/MainServer/conf/ServerHost.conf配置文件中

（7）进入~/release/MainServer目录中，./MainServer即启动主服务器（启动代码处理服务器后，使用kill -3 $(pidof MainServer)命令将机器置为上线状态）

 

### 2. 安装、启动代码处理服务器

（1）解压安装包 

tar -zxvf release.tar.gz

（2）安装jsoncpp库

sudo yum install -y jsoncpp-devel

（3）进入~/release/CodeHandleServer目录中，./CodeHandleServer 端口号（启动代码处理服务器）