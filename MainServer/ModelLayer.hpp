#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <jsoncpp/json/json.h>
#include "DataBases.hpp"
#include "../Common/ComponentTool.hpp"
#include "../Common/LogMessage.hpp"
#include "include/mysql.h"

using std::string;
using std::vector;
using std::ifstream;
using std::shared_ptr;
using std::endl;
using namespace ComponentTool;
using namespace DailyRecord;
using namespace DataBases;

namespace ModelLayer 
{
    struct Question {
        string _number;
        string _title;
        string _difficulty;
        string _describe;
        string _headContent;
        string _tailContent;
        int _cpuLimit;
        int _memoryLimit;
    };

    struct Job {
        string _number;//无法只通过公司名或只通过职位确定一个具体工作
        string _name;
        string _position;//职位
        string _salary;//薪资
        string _address;
        string _describe;
        string _phone;
    };

    const string tableName = "questionlist";
    const string host = "127.0.0.1";//MySQL所在主机ip地址(本地回环)
    // const string user = "bjy";
    // const string password = "Bjy591888@";
    // const string db = "olinejudge";
    const unsigned int port = 3306;//MySQL开放端口
    class Model
    {
    public:
        Model():_database(new DateBase) {}
        ~Model() {}

    public:
        bool Register(const string& inputJson, string &outputJson)//注册
        {
            //获取用户信息
            Json::Value inputValue;
            Json::Reader reader;
            reader.parse(inputJson, inputValue);
            string user = inputValue["User"].asString();
            string password = inputValue["Password"].asString();

            //构建sql语句
            std::string sql = "insert into user(user, password) values('" + user + "','" + password + "');";

            //执行sql语句，成功则用户合法
            Json::Value outputValue;
            if (_database->InsertData(sql)) {
                outputValue["Result"] = "success";
                Json::FastWriter writer;
                outputJson = writer.write(outputValue);
                LOG(NORMAL) << "账号:" << user << " 注册成功, 密码:" << password << endl;
                return true;
            }
            else {
                outputValue["Result"] = "fail";
                Json::FastWriter writer;
                outputJson = writer.write(outputValue);
                LOG(NORMAL) << "注册失败, 用户已存在" << endl; 
                return false;
            }
        }
        int SignIn(const string& inputJson, const string tableName, string& outputJson)
        {
            //获取用户信息
            Json::Value inputValue;
            Json::Reader reader;
            reader.parse(inputJson, inputValue);
            string user = inputValue["User"].asString();
            string password = inputValue["Password"].asString();

            //构建sql语句
            string sql = "select * from " + tableName + " where user='" + user + "';";
            string result;
            int userId = _database->SelectFromUser(sql, password, result);
            Json::Value outputValue;
            outputValue["Result"] = result;
            Json::FastWriter writer;
            outputJson = writer.write(outputValue);

            //判断用户的合法性
            if (userId > 0) {
                LOG(NORMAL) << "用户:" << user << " 登录成功" << endl;
            }
            else {
                if (result == "1") LOG(NORMAL) << "用户不存在" << endl;
                else if (result == "2") LOG(NORMAL) << "用户密码错误" << endl;
                else LOG(ERROR) << "数据库代码编写存在错误" << endl;
            }
            return userId;
        }
        bool ForgetPassword(const string& inputJson, string& outputJson)
        {
            //获取用户信息
            Json::Value inputValue;
            Json::Reader reader;
            reader.parse(inputJson, inputValue);
            string user = inputValue["user"].asString();
            string password = inputValue["password"].asString();

            string sql = "select * from user where user='" + user + "';"; //构建sql语句
            
            //判断用户是否存在
            string result;
            _database->SelectFromUser(sql, password, result);

            bool ret = true;
            if (result == "1") {
                LOG(MORMAL) << "用户:" << user << "不存在" << endl;;
                ret = false;
            }
            else if (result == "") {
                LOG(ERROR) << "数据库代码编写存在错误" << endl;
                ret = false;
            }
            else {//用户存在
                string sql = "update user set password='" + password + "' where user = '" + user + "';";
                result = "";//重复利用该变量
                if (_database->Update(sql, result)) {
                    LOG(NORMAL) << "用户:" << user << " 更新密码成功" << endl;
                }
                else {
                    LOG(WARNING) << "数据库代码编写存在错误" << endl;
                    ret = false; // 开发人员解决
                }
            }

            Json::Value outputValue;
            outputValue["result"] = result;
            Json::FastWriter writer;
            outputJson = writer.write(outputValue);
            return ret;
        }

    public:
        void AddQuestion(const string& inputJson, string& outputJson) {
            _database->AddQuestion(inputJson, outputJson);
        }
        void AddJob(const string& inputJson, string& outputJson) {
            _database->AddJob(inputJson, outputJson);
        }
        
    public:
        string GetUserName(int id, const string& tableName) {
            return _database->GetUserName(id, tableName);
        }

    public:
        bool ObtainAllQuestions (vector<Question>* allQuetions) {
            string sql = "select * from questionlist";
            return GetDataFromMySQL_one(sql, allQuetions);
        }
        bool ObtainDesignatedQuestion(const string& number, Question* question) {
            string sql = "select * from questionlist where number=" + number;
            vector<Question> output;
            if(GetDataFromMySQL_one(sql, &output) && output.size() == 1) {
                *question = output[0];
                return true;
            }
            return false;
        }
        bool ObtainAllJob(vector<Job>* jobs) {
            string sql = "select * from joblist";
            return GetDataFromMySQL_second(sql, jobs);
        }
        bool ObtainDesignatedJob(const string& number, Job* job) {
            string sql = "select * from joblist where number=" + number;
            vector<Job> output;
            if(GetDataFromMySQL_second(sql, &output) && output.size() == 1) {
                *job = output[0];
                return true;
            }
            return false;
        }
    private:
        bool GetDataFromMySQL_one(const string& sql, vector<Question>* output) {
            MYSQL_RES* result = nullptr;
            if(!_database->QueryMysql(sql, result)) return false;

            int row = mysql_num_rows(result);

            for(int i = 0; i < row; ++i) {
                Question question;
                MYSQL_ROW lineMessage = mysql_fetch_row(result);
                question._number = lineMessage[0];
                question._title = lineMessage[1];
                question._difficulty = lineMessage[2];
                question._describe = lineMessage[3];
                question._headContent = lineMessage[4];
                question._tailContent = lineMessage[5];
                question._cpuLimit = atoi(lineMessage[6]);
                question._memoryLimit = atoi(lineMessage[7]);
                (*output).push_back(question);
            }
            
            free(result);
            return true;
        }

        bool GetDataFromMySQL_second(const string& sql, vector<Job>* output) {
            MYSQL_RES* result = nullptr;
            if(!_database->QueryMysql(sql, result)) return false;

            int row = mysql_num_rows(result);

            for(int i = 0; i < row; ++i) {
                Job job;
                MYSQL_ROW lineMessage = mysql_fetch_row(result);
                job._number = lineMessage[0];
                job._name = lineMessage[1];
                job._position = lineMessage[2];
                job._salary = lineMessage[3];
                job._address = lineMessage[4];
                job._describe = lineMessage[5];
                job._phone = lineMessage[6];
                (*output).push_back(job);
            }
            
            free(result);
            return true;
        }
    private:
        shared_ptr<DateBase> _database;
    };
}  
