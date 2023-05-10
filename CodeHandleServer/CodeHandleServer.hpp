#pragma once
#include "CompileService.hpp"
#include "RunService.hpp"
#include "../Common/ComponentTool.hpp"
#include "../Common/LogMessage.hpp"
#include <jsoncpp/json/json.h>

namespace CodeHandleServer
{
    using namespace RunService;
    using namespace CompileService;

    class CodeHandle
    {
        /***************************************************************
         * 输入Json串：
         *   Code        : 待处理代码
         *   Input       : 用户提交代码的标准输入内容
         *   CpuLimit    : CPU限制
         *   MemoryLimit : 内存限制
         *
         * 输出Json串：
         *   StatusCode  : 状态码
         *   Reason      : 结果原因
         *   Stdout      : 用户提交代码的运行正确结果
         *   Stderr      : 用户提交代码的运行错误结果
         ***************************************************************/
    public:
        static void StartServer(const string &inputJson, string* outputJson)
        {
            // 反序列化
            Json::Value inputValue;
            Json::Reader reader;
            reader.parse(inputJson, inputValue);
            string code = inputValue["Code"].asString();
            string input = inputValue["Input"].asString();
            string cpuLimit = inputValue["CpuLimit"].asString();
            string memoryLimit = inputValue["MemoryLimit"].asString();

            // 序列化
            if (code.size() == 0) {
                *outputJson = SerializeHandler(-1, ""); // 用户代码为空
                return;
            }

            string fileName = FileTool::GetUniqueFileName();
            if (!FileTool::WriteToFile(PathTool::StructureCompletePath_Source(fileName), code)) {
                *outputJson = SerializeHandler(-2, fileName); // 未知错误(写入失败)
                FileTool::CleanUpAllTemporaryFiles(fileName);
                return;
            }
            if (!Compile::CompileCode(fileName)) {
                *outputJson = SerializeHandler(-3, fileName); // 编译失败
                FileTool::CleanUpAllTemporaryFiles(fileName);
                return;
            }
            int RunResult = Run::RunCode(fileName, atoi(cpuLimit.c_str()), atoi(memoryLimit.c_str()));
            if (RunResult > 0) {
                *outputJson = SerializeHandler(RunResult, fileName); // 程序异常(运行时崩溃)
                FileTool::CleanUpAllTemporaryFiles(fileName);
                return;
            }
            else if (RunResult == 0) {
                *outputJson = SerializeHandler(0, fileName); // 程序正常运行
                FileTool::CleanUpAllTemporaryFiles(fileName);
                return;
            }
            else {
                *outputJson = SerializeHandler(-2, fileName); // 未知错误
                FileTool::CleanUpAllTemporaryFiles(fileName);
                return;
            }
        }

    private:
        //序列化操作
        static string SerializeHandler(const int statusCode, const string fileName)
        {
            Json::Value outputValue;
            outputValue["StatusCode"] = statusCode;
            outputValue["Reason"] = StatusTool::StatusCodeToReason(statusCode, fileName);
            if (statusCode == 0)
            {
                string stdout;
                FileTool::ReadFromFile(PathTool::StructureCompletePath_Stdout(fileName), &stdout, true);
                outputValue["Stdout"] = stdout;
                string stdin;
                FileTool::ReadFromFile(PathTool::StructureCompletePath_Stderr(fileName), &stdin, true);
                outputValue["Stderr"] = stdin;
            }
            Json::FastWriter writer;
            return writer.write(outputValue);
        }
    };
}