#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <mutex>
#include <cassert>
#include <jsoncpp/json/json.h>
#include "ModelLayer.hpp"
#include "ViewLayer.hpp"
#include "LoadBalancer.hpp"
#include "../Common/httplib.h"
#include "../Common/ComponentTool.hpp"
#include "../Common/LogMessage.hpp"

using namespace ComponentTool;
using namespace DailyRecord;
using namespace httplib;
using namespace ModelLayer;
using namespace ViewLayer;
using namespace LoadBalance;
using std::sort;
using std::string;
using std::vector;
using std::mutex;
using std::unique_lock;
using std::endl;

namespace Controller
{   
    class Control
    {
    public:
        bool GetAllQuestions(string* html) {
            vector<Question> allQuestions;
            if(_model.ObtainAllQuestions(&allQuestions)) {
                sort(allQuestions.begin(), allQuestions.end(), [](const Question& question_1, const Question& question_2){
                    return atoi(question_1._number.c_str()) < atoi(question_2._number.c_str());
                });
                _view.AllQuestionsRomanceHtml(allQuestions, html);
                return true;
            }
            else {
                *html = "Failed to obtain question bank and form question list";
                return false;
            }
        }
        bool GetDesignatedQuestion(const string& number, string* html) {
            Question question;
            if(_model.ObtainDesignatedQuestion(number, &question)) {
                _view.DesignatedQuestionRomanceHtml(question, html);
                return true;
            }
            else {
                *html = "Question" + number + "does not exist";
                return false;
            }
        }
        bool GetAllJob(string* html) {
            vector<Job> allJob;
            if(_model.ObtainAllJob(&allJob)) {
                sort(allJob.begin(), allJob.end(), [](const Job& job_1, const Job& job_2){
                    return atoi(job_1._salary.c_str()) > atoi(job_2._salary.c_str());
                });
                _view.AllJobRomanceHtml(allJob, html);
                return true;
            }
            else {
                *html = "Failed to obtain company bank and form company list";
                return false;
            }
        }
        bool GetDesignatedJob(const string& number, string* html) {
            Job job;
            if(_model.ObtainDesignatedJob(number, &job)) {
                _view.DesignatedJobRomanceHtml(job, html);
                return true;
            }
            else {
                *html = "Job" + number + "does not exist";
                return false;
            }
        }
    public:
        /***************************************************************
        * inputJson串：
        *   Code        : 用户提交代码
        *   Input       : 用户提交代码的标准输入内容
        ***************************************************************/
        void JudgmentCode(const string& number, const string& inputJson, string* outputJson) 
        {
            //通过题号获取题目信息
            Question question;
            _model.ObtainDesignatedQuestion(number, &question);
            //将用户inputJson串进行反序列化
            Json::Reader reader;
            Json::Value inputValue;
            reader.parse(inputJson, inputValue);
            //构建codeHandleString
            Json::Value outputValue;
            outputValue["Input"] = inputValue["Input"].asString();
            outputValue["Code"] = inputValue["Code"].asString() + "\n" + question._tailContent;//拼接用户代码与测试用例代码
            outputValue["CpuLimit"] = question._cpuLimit;
            outputValue["MemoryLimit"] = question._memoryLimit;
            Json::FastWriter writer;
            string codeHandleString = writer.write(outputValue);

            //不断选择主机
            while(true) 
            {
                size_t hostID = 0;
                ServerHost* serverHost = nullptr;
                if(!_loadBalancer.SelectServiceHost(&hostID, &serverHost)) break;//所有主机不在线          

                //充当代码处理服务器的客户端
                httplib::Client client(serverHost->GetIP(), serverHost->GetPort());
                serverHost->IncreaseLoad();
                LOG(NORMAL) << "Successfully selected the host with the least load, hostID:" << hostID << " ip:" <<
                serverHost->GetIP() << " port:" << serverHost->GetPort() << " 当前主机负载为:" << serverHost->GetLoadSituation() << endl;
                if(httplib::Result ret = client.Post("/codehandle", codeHandleString, "application/json; charset=utf-8")) 
                {
                    if (ret->status == 200) {//http请求成功时状态码为200
                        *outputJson = ret->body;
                        serverHost->ReduceLoad();
                        LOG(NORMAL) << "Request code processing server successful" << endl;
                        break;
                    }
                    serverHost->ReduceLoad();
                }
                else//主机处于离线状态，请求失败
                {
                    LOG(ERROR) << "Request code processing server failed, hostID:" << hostID << " ip: " << \
                        serverHost->GetIP() << " port: " << serverHost->GetPort() << endl;
                    serverHost->ReduceLoad();
                    _loadBalancer.OfflineServerHost(hostID);
                    _loadBalancer.ShowServerHost();//for Debug
                }
            }
        }

    public:
        void RestoreHosts() { _loadBalancer.OnlineServerHost(); }

    public:
        bool UserRegistration(const string& inputJson, string& outputJson) {
            return _model.Register(inputJson, outputJson);
        }
        int UserSignIn(const string& inputJson, const string tableName, std::string &outputJson) {
            return _model.SignIn(inputJson, tableName, outputJson);
        }
        bool ForgetPassword(const string& inputJson, string& outputJson) {
            return _model.ForgetPassword(inputJson, outputJson);
        }

    public:
        void AddQuestion(const string& inputJson, string& outputJson) {
            _model.AddQuestion(inputJson, outputJson);
        }
        void AddJob(const string& inputJson, string& outputJson) {
             _model.AddJob(inputJson, outputJson);
        }

    public:
        string GetUserName(int id, const string& tableName) {
            return _model.GetUserName(id, tableName);
        }

    private:
        Model _model;
        View _view;
        LoadBalancer _loadBalancer;
    };
}
