#include "CodeHandleServer.hpp"
#include "../Common/httplib.h"
#include "../Common/Daemon.hpp"
using namespace CodeHandleServer;
using namespace httplib;
using std::cerr;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cerr << "\nUsage: " << argv[0] << " prot" << endl;
        return 0;
    }
    int fd = DaemonSelf(atoi(argv[1]));
    
    Server server;

    // 提供编译运行服务
    server.Post("/codehandle", [](const Request& req, Response& rsp) {
        std::string in_json = req.body;
        std::string out_json;
        if (!in_json.empty()) {
            CodeHandle::StartServer(in_json, &out_json);
            rsp.set_content(out_json, "application/json; charset=utf-8");
        }
    });

    server.listen("0.0.0.0", atoi(argv[1])); // 启动http服务

    close(fd);
    return 0;
}