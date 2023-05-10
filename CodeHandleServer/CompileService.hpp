#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../Common/ComponentTool.hpp"
#include "../Common/LogMessage.hpp"
using namespace ComponentTool;
using namespace DailyRecord;
using std::string;
using std::endl;

namespace CompileService
{
    class Compile
    {
    public:
        static bool CompileCode(const string& fileName) 
        {
            pid_t pid = fork();
            if(pid == 0) 
            {
                //将错误信息写入.ComErr文件
                int errFileFd = open(PathTool::StructureCompletePath_CompileErr(fileName).c_str(), O_CREAT | O_RDWR, 0666);
                if(errFileFd < 0) {
                    LOG(WARING) << "Standard error file not successfully generated" << endl;
                    exit(1);
                }
                dup2(errFileFd, STDERR_FILENO);

                //execlp函数调用的可执行程序在环境变量中查找，本地服务器g++安装后，一定配置在环境变量中
                string temp1 = PathTool::StructureCompletePath_Source(fileName).c_str();
                string temp2 = PathTool::StructureCompletePath_Exe(fileName).c_str();
                const char* str1 = temp1.c_str();
                const char* str2 = temp2.c_str();
                const char* argvs[] = {"g++", str1, "-o", str2, "-std=c++11", "-D", "CODEHANDLE", NULL};
                execvp("g++", (char* const*)argvs);//进程替换不影响文件描述符表

                LOG(ERROR) << "Failed to start g++ for compilation" << endl;//若进程替换成功不会执行此代码
                exit(2);
            }
            else if(pid > 0) 
            {
                waitpid(pid, nullptr, 0);//阻塞等待
                if(FileTool::FileIsExistence(PathTool::StructureCompletePath_Exe(fileName))) {
                    LOG(NORMAL) << PathTool::StructureCompletePath_Source(fileName).c_str() 
                    << " Compilation succeeded in generating " << PathTool::StructureCompletePath_Exe(fileName).c_str() << endl;
                    return true;
                }
                LOG(ERROR) << "Compilation failed" << endl;
            }
            else 
            {
                LOG(ERROR) << "Fork child process error" << endl;
                return false;
            }
            return false;
        }
    };
}