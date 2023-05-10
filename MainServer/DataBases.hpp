#pragma once
#include <iostream>
#include <vector>
#include <mutex>
#include <cassert>
#include <jsoncpp/json/json.h>
#include "include/mysql.h"
#include "../Common/LogMessage.hpp"
#include "../Common/ComponentTool.hpp"

using namespace DailyRecord;
using namespace ComponentTool;
using std::vector;
using std::unique_lock;
using std::mutex;
using std::endl;

namespace DataBases
{
    const string host = "127.0.0.1";//MySQL所在主机ip地址(本地回环)
    const string db = "olinejudge";
    const unsigned int port = 3306;//MySQL开放端口
    class DateBase
    {
    public:
        DateBase():_mysql_fp(mysql_init(nullptr)), _pmutex(new mutex) { assert(ConnectMysql()); }
        ~DateBase() {
            mysql_close(_mysql_fp);
            delete _pmutex;
        }
    public:
        bool Update(const string& sql, string& out) {
            if (mysql_query(_mysql_fp, sql.c_str()) != 0) {
                LOG(WARINNG) << sql + " 执行失败" << endl;
                return false;
            }
            out = "0";
            return true;
        }
        bool InsertData(const string &sql) {
            if (mysql_query(_mysql_fp, sql.c_str()) != 0) {
                LOG(WARINNG) << sql << " 执行失败" << endl;
                return false;
            }
            return true;
        }
        bool QueryMysql(const string &sql, MYSQL_RES *&result)
        {
            unique_lock<mutex> lock(*_pmutex);
            int ret = mysql_query(_mysql_fp, sql.c_str());
            if (ret != 0) {
                LOG(WARINNG) << sql << " 执行失败" << endl;
                return false;
            }
            result = mysql_store_result(_mysql_fp);
            return true;
        }
        int SelectFromUser(const string& sql, const string& password, string& out)
        {
            int ret = mysql_query(_mysql_fp, sql.c_str());
            if (ret != 0) {
                LOG(WARINNG) << sql << " 执行失败" << endl;;
                return -1;
            }
            MYSQL_RES *result = mysql_store_result(_mysql_fp);
            int row = mysql_num_rows(result);
            if (row == 0) {
                out = "1";
                return -2;
            }
            MYSQL_ROW lineMessage = mysql_fetch_row(result);
            if (lineMessage[2] != password) {
                out = "2";
                return -3;
            }
            out = "0";
            free(result);
            return atoi(lineMessage[0]);
        }

    public:
        void AddQuestion(const string& inputJson, string& outputJson)
        {
            //获取用户信息
            Json::Value inputValue;
            Json::Reader reader;
            reader.parse(inputJson, inputValue);

            string title = inputValue["Title"].asString();
            string difficulty = inputValue["Difficulty"].asString();
            string describe = inputValue["Describe"].asString();
            string headContent = inputValue["HeadContent"].asString();
            string tailContent = inputValue["TailContent"].asString();
            string cpuLimit = inputValue["CpuLimit"].asString();
            string memoryLimit = inputValue["MemoryLimit"].asString();
            string sql = "insert into questionlist(title, difficulty, `describe`, headContent, tailContent, CpuLimit, MemoryLimit) values('";
            sql += title + "', '";
            sql += difficulty + "', '";
            sql += describe + "', '";
            sql += headContent + "', '";
            sql += tailContent + "', ";
            sql += cpuLimit + ", ";
            sql += memoryLimit + ");";

            std::cout << sql << endl;

            Json::Value outputValue;
            if (InsertData(sql)) {
                LOG(NORMAL) << "题目添加成功" << endl;
                outputValue["Result"] = "success";
            }
            else {
                LOG(NORMAL) << "题目添加失败" << endl;
                outputValue["Result"] = "fail";
            }
            Json::FastWriter writer;
            outputJson = writer.write(outputValue);
        }

        void AddJob(const string& inputJson, string& outputJson) {
            //获取工作信息
            Json::Value inputValue;
            Json::Reader reader;
            reader.parse(inputJson, inputValue);

            string name = inputValue["Name"].asString();
            string position = inputValue["Position"].asString();
            string salary = inputValue["Salary"].asString();
            string address = inputValue["Address"].asString();
            string describe = inputValue["Describe"].asString();
            string phone = inputValue["Phone"].asString();
            string sql = "insert into joblist(`name`, `position`, `salary`, `address`, `describe`, `phone`) values('";
            sql += name + "', '";
            sql += position + "', '";
            sql += salary + "', '";
            sql += address + "', '";
            sql += describe + "',' ";
            sql += phone + "');";

            std::cout << sql << endl;

            Json::Value outputValue;
            if (InsertData(sql)) {
                LOG(NORMAL) << "添加工作成功" << endl;
                outputValue["Result"] = "success";
            }
            else {
                LOG(NORMAL) << "工作添加失败" << endl;
                outputValue["Result"] = "fail";
            }
            Json::FastWriter writer;
            outputJson = writer.write(outputValue);
        }

    public:
        string GetUserName(int id, const string& tableName)
        {
            string sql = "select user from " + tableName + " where UserId" + " = ";
            sql += std::to_string(id) + ";";

            int ret = mysql_query(_mysql_fp, sql.c_str());
            if (ret != 0) {
                LOG(WARNING) << sql << "执行失败" << endl;
                return "";
            }
            MYSQL_RES *result = mysql_store_result(_mysql_fp);
            int row = mysql_num_rows(result);
            if (row == 0) {
                LOG(ERROR) << "无结果" << endl;
                return "";
            }

            MYSQL_ROW lineMessage = mysql_fetch_row(result);
            free(result);
            return lineMessage[0];
        }

    private:
        bool ConnectMysql()
        {
            string temp;
            FileTool::ReadFromFile("./conf/mysqluser.conf", &temp, true);
            vector<string> ret = SplitTool::SplitString(temp, "\n");
            string user = ret[0];
            string password = ret[1];

            if (nullptr == mysql_real_connect(_mysql_fp, host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, nullptr, 0)) {
                LOG(ERROR) << " Failed to access MySQL" << endl;
                return false;
            }
            mysql_set_character_set(_mysql_fp, "utf8");
            LOG(NORMAL) << "Successfully linked MySQL" << endl;
            return true;
        }

    private:
        MYSQL* _mysql_fp;
        mutex* _pmutex;
    };
}