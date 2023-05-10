#include <iostream>
#include <string>
#include <cstdlib>                                                                                                                                                               
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using std::string;
 
static int DaemonSelf(int number)
{
    //设置文件掩码
    umask(0);
    //fork后终止父进程,保证子进程不为进程组组长
    if(fork() > 0) exit(0);
    //创建新会话
    setsid();
    //fork后终止父进程，保持子进程不是会话首进程，从而保证后续不会再和其他终端相关联
    //（不是必须的，防御性编程）
    if(fork() > 0) exit(0);
    //标准输入、标准输出、标准错误重定向
    string fileName = "log" + std::to_string(number);
    int fileFd = open(fileName.c_str(), O_CREAT | O_RDWR | O_APPEND, 0666);
    if(fileFd > 0) {
        dup2(fileFd, 0);
        dup2(fileFd, 1);
        dup2(fileFd, 2);
    }
    return fileFd;
}
