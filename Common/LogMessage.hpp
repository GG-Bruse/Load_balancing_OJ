#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include "../Common/ComponentTool.hpp"
using std::string;
using ComponentTool::TimeTool;

namespace DailyRecord
{
    #define LOG(level) Log(#level, __FILE__, __LINE__)
    //日志级别
    enum {
        DEBUG, NORMAL,
        WARNING, ERROR,
        FATAL
    };
	std::ostream& Log(const string& level, const string& fileName, int lineNum)
    {
        std::string message = "[" + level + "]";
        message += "[" + fileName + "-" + std::to_string(lineNum) + "]";
        message += "[" + TimeTool::GetTime_MonToSec() + "]";

        std::cout << message; // 存入缓冲区，不刷新待填充报错信息
        return std::cout;
    }
}
