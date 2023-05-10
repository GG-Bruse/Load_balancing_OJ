#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <atomic>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <boost/algorithm/string.hpp>
using std::string;
using std::vector;
using std::to_string;
using std::ofstream;
using std::ifstream;
using std::atomic_int;

namespace ComponentTool
{
    const string tempPath = "./TempFile/";
    class PathTool
    {
    public:
        //将文件名字符串构建成包含路径、源文件后缀的完整字符串
        static string StructureCompletePath_Source(const string& fileName) {
            return StructureCompletePath(fileName, ".cpp");
        }
        //将文件名字符串构建成包含路径、编译错误文件后缀的完整字符串
        static string StructureCompletePath_CompileErr(const string& fileName) {
            return StructureCompletePath(fileName, ".ComErr");
        }
        //将文件名字符串构建成包含路径、可执行文件后缀的完整字符串
        static string StructureCompletePath_Exe(const string& fileName) {
            return StructureCompletePath(fileName, ".exe");
        }
        //将文件名字符串构建成包含路径、标准输入文件后缀的完整字符串
        static string StructureCompletePath_Stdin(const string& fileName) {
            return StructureCompletePath(fileName, ".in");
        }
        //将文件名字符串构建成包含路径、标准输出文件后缀的完整字符串
        static string StructureCompletePath_Stdout(const string& fileName) {
            return StructureCompletePath(fileName, ".out");
        }
        //将文件名字符串构建成包含路径、标准错误文件后缀的完整字符串
        static string StructureCompletePath_Stderr(const string& fileName) {
            return StructureCompletePath(fileName, ".errMsg");
        }
    private:
        static string StructureCompletePath(const string& fileName, const string& suffix) {
            return tempPath + fileName + suffix;
        }
    };



    class TimeTool
    {
    public:
        static string GetTime_MonToSec() {
            const time_t timestamp = time(nullptr);
            struct tm* local_time = localtime(&timestamp);
            return to_string(local_time->tm_mon + 1) + "_" + to_string(local_time->tm_mday) + "_" + 
            to_string(local_time->tm_hour) + "_" +to_string(local_time->tm_min) + "_" + to_string(local_time->tm_sec);
        }
    public:
        //获取毫秒级时间戳
        static string GetMillisecondLevelTimestamp() {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            return to_string(tv.tv_sec * 1000 + tv.tv_usec / 1000);
        }
    };



    class FileTool
    {
    public:
        //判断文件是否存在，也可通过打开文件是否成功来判断文件是否存在，但调用stat可获取文件属性,便于后续扩展
        static bool FileIsExistence(const string& pathName) {
            struct stat _stat;
            if(stat(pathName.c_str(), &_stat) == 0) return true;
            else return false;
        }
    public:
        //获取唯一文件名
        static string GetUniqueFileName() {
            static atomic_int fildId(0);
            ++fildId;
            string milliSecond = TimeTool::GetMillisecondLevelTimestamp();
            string UniqueFileId = to_string(fildId);
            return UniqueFileId + "." + milliSecond;
        }
    public:
        //文件读写
        static bool WriteToFile(const string& targetFile, const string& fileContent) 
        {
            ofstream output(targetFile);
            if(!output.is_open()) return false;
            output.write(fileContent.c_str(), fileContent.size());
            output.close();
            return true;
        }
        static bool ReadFromFile(const string& targetFile, string* fileContent, bool keepFlag = false) 
        {
            ifstream input(targetFile);
            if(!input.is_open()) return false;
            string line;
            while(getline(input, line)) {
                (*fileContent) += line;
                (*fileContent) += (keepFlag == true ? "\n" : "");
            }
            input.close();
            return true;
        }
    public:
        //清理所有临时文件
        static void CleanUpAllTemporaryFiles(const string& fileName) 
        {
            string srcFile = PathTool::StructureCompletePath_Source(fileName);
            string comErrFile = PathTool::StructureCompletePath_CompileErr(fileName);
            string exeProgram = PathTool::StructureCompletePath_Exe(fileName);
            string inFile = PathTool::StructureCompletePath_Stdin(fileName);
            string outFile = PathTool::StructureCompletePath_Stdout(fileName);
            string errFile = PathTool::StructureCompletePath_Stderr(fileName);
            if(FileIsExistence(srcFile)) unlink(srcFile.c_str());
            if(FileIsExistence(comErrFile)) unlink(comErrFile.c_str());
            if(FileIsExistence(exeProgram)) unlink(exeProgram.c_str());
            if(FileIsExistence(inFile)) unlink(inFile.c_str());
            if(FileIsExistence(outFile)) unlink(outFile.c_str());
            if(FileIsExistence(errFile)) unlink(errFile.c_str());
        }
    };



    class ResourceLimitTool
    {
    public:
        //限制进程被CPU调度的时间
        static void SetProcessCpuLimit(size_t cpuLimit) {
            struct rlimit limit;
            limit.rlim_cur = cpuLimit;
            limit.rlim_max = RLIM_INFINITY;
            setrlimit(RLIMIT_CPU, &limit);
        }
    public:
        //限制进程所占内存大小
        static void SetProcessMemoryLimit(size_t memoryLimit) {
            struct rlimit limit;
            limit.rlim_cur = memoryLimit;
            limit.rlim_max = RLIM_INFINITY;
            setrlimit(RLIMIT_AS, &limit);
        }
    };



    class StatusTool
    {
    public:
        static string StatusCodeToReason(int statusCode, const string fileName)
        {
            string reason;
            switch (statusCode)
            {
            case 0:
                reason = "Compile and run successfully";
                break;
            case -1:
                reason = "The submission code is empty";
                break;
            case -2:
                reason = "Unknown error occurred";
                break;
            case -3:
                FileTool::ReadFromFile(PathTool::StructureCompletePath_CompileErr(fileName), &reason, true);
                break;
            case SIGXCPU:
                reason = "CPU time limit exceeded";
                break;
            case SIGABRT:
                reason = "Memory limit exceeded";
                break;
             case SIGFPE:
                reason = "Segmentation fault";
                break;
             case SIGSEGV:
                reason = "Segmentation fault";
                break;
            default: 
                reason = "Unknown error occurred " + to_string(statusCode);
                break;
            }
            return reason;
        }
    };



    class SplitTool
    {
    public:
        static vector<string> SplitString(const string& targetString, const string delimiter/*分隔符*/) {
            vector<string> ret; 
            boost::split(ret, targetString, boost::is_any_of(delimiter), boost::algorithm::token_compress_on);
            return ret;
        }
    };
}