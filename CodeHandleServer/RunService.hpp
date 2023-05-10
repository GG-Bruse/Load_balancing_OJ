#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../Common/LogMessage.hpp"
#include "../Common/ComponentTool.hpp"
using namespace ComponentTool;
using namespace DailyRecord;
using std::string;
using std::endl;

namespace RunService
{
    class Run
    {
    public:
        //返回值: 若大于0，则说明程序异常，返回对应的信号编号
        //        若等于0，则说明程序正常运行
        //        若小于0，则发生内部错误(fork、open失败等)
        static int RunCode(const string& fileName, size_t cpuLimit/*sec*/, size_t memoryLimit/*KB*/) 
        {
            string exeProgram = PathTool::StructureCompletePath_Exe(fileName);
            string inFile = PathTool::StructureCompletePath_Stdin(fileName);
            string outFile = PathTool::StructureCompletePath_Stdout(fileName);
            string errFile = PathTool::StructureCompletePath_Stderr(fileName);
            int inFd = open(inFile.c_str(), O_CREAT | O_WRONLY, 0666);
            int outFd = open(outFile.c_str(), O_CREAT | O_WRONLY, 0666);
            int errFd = open(errFile.c_str(), O_CREAT | O_WRONLY, 0666);
            if(inFd < 0 || outFd < 0 || errFd < 0) {
                LOG(ERROR) << "Fail to open file" << endl;
                return -1;
            }

            pid_t pid = fork();
            if(pid == 0)
            {
                dup2(inFd, STDIN_FILENO);
                dup2(outFd, STDOUT_FILENO);
                dup2(errFd, STDERR_FILENO);
                ResourceLimitTool::SetProcessCpuLimit(cpuLimit);
                ResourceLimitTool::SetProcessMemoryLimit(memoryLimit * 1024);
                execl(exeProgram.c_str(), exeProgram.c_str(), nullptr);
                LOG(ERROR) << "Failed to start g++ for compilation" << endl;
                exit(1);
            }
            else if(pid > 0)
            {
                close(inFd), close(outFd), close(errFd);
                int status = 0;
                waitpid(pid, &status, 0);//阻塞等待
                LOG(NORMAL) << "Run Complete, signo:" << (status & 0x7F) << endl;
                return status & 0x7F;
            }
            else
            {
                close(inFd), close(outFd), close(errFd);
                LOG(ERROR) << "Fork child process error" << endl;
                return -1;
            }
        }
    };
}