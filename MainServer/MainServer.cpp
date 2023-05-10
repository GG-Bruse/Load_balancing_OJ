#include <iostream>
#include <string>
#include <signal.h>
#include "../Common/httplib.h"
#include "../Common/Daemon.hpp"
#include "Controller.hpp"
#include "Session.hpp"
#include "UserManage.hpp"

using namespace httplib;
using namespace Controller;
using namespace session;
using namespace usermanage;
using std::shared_ptr;
using std::string;

static Control* p_controller = nullptr;
void Resume(int signo) { p_controller->RestoreHosts(); }

int main()
{
    signal(SIGQUIT, Resume);

    int fd = DaemonSelf(0);

    Control controller;
    p_controller = &controller;
    
    shared_ptr<AllSessionInfo> allSess(new AllSessionInfo);
    shared_ptr<UserManage> manager(new UserManage);
    Server server;
    server.set_base_dir("./wwwroot");

    //用户注册
    server.Post("/register", [&controller](const Request& request, Response& response) {
        LOG(INFO) << "Get a register request" << endl;;
        string outputJson;
        controller.UserRegistration(request.body, outputJson);
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });

    //登录
    server.Post("/sign", [&controller, &allSess, &manager](const Request& request, Response& response) {
        LOG(NORMAL) << "Get a sign request" << endl;
        string outputJson;
        int userId = controller.UserSignIn(request.body, "user"/*tableName*/, outputJson);
        string temp;
        if (userId > 0) {
            Session sess(request.body, userId, "user");
            string sessionId = sess.GetSessionId();
            temp = "JSESSION=" + sessionId;
            allSess->SetSessionInfo(sessionId, sess);
            //将用户添加到管理
            manager->AddUserToSet(userId, 1);
        }
        response.set_header("Set-Cookie", temp.c_str());//返回cookie
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });

    // 修改密码
    server.Post("/forget", [&controller](const Request& request, Response& response) {
        LOG(NORMAL) << "Get a forget password request" << endl;
        string outputJson;
        controller.ForgetPassword(request.body, outputJson);
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });

    //管理员登录
    server.Post("/administrator", [&controller, &allSess, &manager](const Request& request, Response& response) {
        LOG(MORMAL) << "get a administrator sign request" << endl;
        string outputJson;
        int administratorId = controller.UserSignIn(request.body, "administrators", outputJson);
        std::cout << administratorId << endl;
        std::cout << outputJson << endl;
        string temp;
        if (administratorId > 0) {
            Session sess(request.body, administratorId, "administrator");
            string session_id = sess.GetSessionId();
            temp = "JSESSION=" + session_id;
            allSess->SetSessionInfo(session_id, sess);
            manager->AddUserToSet(administratorId);
        }
        response.set_header("Set-Cookie", temp.c_str());
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });

    //获取所有题目列表
    server.Get("/all_questions", [&controller](const Request& request, Response& response) {
        string html;
        controller.GetAllQuestions(&html);
        response.set_content(html, "text/html; charset=utf-8");
    });
    //根据题目编号获取题目内容
    server.Get(R"(/question/(\d+))", [&controller](const Request& request, Response& response){
        string html;
        string number = request.matches[1];
        controller.GetDesignatedQuestion(number, &html);
        response.set_content(html, "text/html; charset=utf-8");
    });
    //用户提交代码，进行判题
    server.Post(R"(/codehandle/(\d+))", [&controller, &allSess](const Request& request, Response& response){
        string number = request.matches[1];
        string outputJson;
        controller.JudgmentCode(number, request.body, &outputJson);
        response.set_content(outputJson, "application/json; charset=utf-8");
    });

    //获取所有工作信息
    server.Get("/job_hunt", [&controller](const Request& request, Response& response){
        string html;
        controller.GetAllJob(&html);
        response.set_content(html, "text/html; charset=utf-8");
    });
    //获取具体工作信息
    server.Get(R"(/job/(\d+))", [&controller](const Request& request, Response& response){
        string html;
        string number = request.matches[1];
        controller.GetDesignatedJob(number, &html);
        response.set_content(html, "text/html; charset=utf-8");
    });

    //添加题目
    server.Post("/add_question", [&controller](const Request& request, Response& response) {
        LOG(NORMAL) << "get a add question request" << endl;
        string outputJson;
        controller.AddQuestion(request.body, outputJson);
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });
    //添加工作
    server.Post("/add_job", [&controller](const Request& request, Response& response) {
        LOG(NORMAL) << "get a add job request" << endl;
        string outputJson;
        controller.AddJob(request.body, outputJson);
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });

    //会话校验
    server.Get("/get_user_id", [&allSess](const Request& request, Response& response) {
        Json::Value outputValue;
        outputValue["UserId"] = allSess->CheckSessionInfo(request);

        string outputJson;
        Json::FastWriter writer;
        outputJson = writer.write(outputValue); 
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });

    //获取用户名
    server.Post("/get_user_name", [&controller, &allSess](const Request& request, Response& response) {
        LOG(INFO) << "get a get username request" << endl;
        Json::Value inputValue;
        Json::Reader reader;
        reader.parse(request.body, inputValue);

        int id = inputValue["UserId"].asInt();
        string tableName = inputValue["tableName"].asString();
           
        Json::Value outputValue;
        outputValue["username"] = controller.GetUserName(id, tableName);
        Json::FastWriter writer;
        string outputJson;
        outputJson = writer.write(outputValue); 
        response.set_content(outputJson, "application/json;charset=utf-8"); 
    });

    server.listen("0.0.0.0", 8080);//启动监听
    close(fd);
    return 0;
}
