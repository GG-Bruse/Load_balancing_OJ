#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <openssl/md5.h>
#include <unordered_map>
#include <cstring>
#include <ctime>
#include <jsoncpp/json/json.h>
#include "../Common/httplib.h"
#include "../Common/ComponentTool.hpp"

using namespace ComponentTool;
using std::string;
using std::unordered_map;
using std::mutex;
using std::unique_lock;

namespace session
{
    class Session
    {
    public:
        Session() {}
        Session(const string& inputJson, int id, const string& identity)
        {
            Json::Value inputValue;
            Json::Reader reader;
            reader.parse(inputJson, inputValue);

            _realStr += inputValue["user"].asString();
            _realStr += inputValue["password"].asString();
            _realStr += identity;
            _realStr += TimeTool::GetMillisecondLevelTimestamp();

            _id = id;
            _identity = identity;
        }
        
        string& GetSessionId() {
            SumMd5();
            return _sessionId;
        }

        int GetID() { return _id; };
    public:
        bool SumMd5()
        {
            MD5_CTX ctx;
            MD5_Init(&ctx);//初始化

            if (MD5_Update(&ctx, _realStr.c_str(), _realStr.size()) != 1) return false; //计算MD5

            unsigned char md5[16] = {0};
            if (MD5_Final(md5, &ctx) != 1) return false;//获取MD5结果数据

            //处理数据
            char tmp[3] = {0};
            char buf[32] = {0};
            for (int i = 0; i < 16; i++) {
                snprintf(tmp, sizeof(tmp) - 1, "%02x", md5[i]);
                strncat(buf, tmp, 2);
            }
            _sessionId = buf;
            return true;
        }

    private:
        string _sessionId; //当前会话的会话id
        string _realStr;   //用来生成会话id的原生字符串
        string _identity;
        int _id;//用户ID
    };


    class AllSessionInfo
    {
    public:
        AllSessionInfo() {}
        ~AllSessionInfo() {}
    public:
        //设置会话信息
        void SetSessionInfo(string& sessionId, Session &sess) {
            unique_lock<mutex> lock(_mapLock);
            _sessionMap.insert(make_pair(sessionId, sess));
        }
        //检查登录会话ID是正常还非法
        int CheckSessionInfo(const Request &request)
        {
            string sessionId = GetSessionId(request);
            if (sessionId == "")  return -1;
            Session sess;
            if (GetSessionInfo(sessionId, &sess) < 0) return -2;
            return sess.GetID(); //若最终会话校验成功，返回该用户id
        }

    private:
        //从请求中获取会话ID
        string GetSessionId(const Request &request)
        {
            string session_kv = request.get_header_value("Cookie");
            vector<string> out;
            out = SplitTool::SplitString(session_kv, "=");//切割字符串
            return out.back();
        }
        //根据sessionId获取会话信息
        bool GetSessionInfo(const string& sessionId, Session* sessionInfo)
        {
            unique_lock<mutex> lock(_mapLock);
            auto iter = _sessionMap.find(sessionId);
            if (iter == _sessionMap.end()) return false;
            if (sessionInfo != nullptr) *sessionInfo = iter->second;
            return true;
        }
    
    private:
        unordered_map<string, Session> _sessionMap;//存储会话ID与会话之间的映射关系
        mutex _mapLock;
    };
}
    