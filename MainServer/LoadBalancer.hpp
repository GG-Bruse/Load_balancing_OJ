#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <mutex>
#include <cassert>
#include "../Common/ComponentTool.hpp"
#include "../Common/LogMessage.hpp"

using namespace ComponentTool;
using namespace DailyRecord;
using std::ifstream;
using std::string;
using std::vector;
using std::mutex;
using std::unique_lock;
using std::endl;

namespace LoadBalance
{
    //代码处理服务主机
    class ServerHost
    {
    public:
        ServerHost():_ip(""),_prot(0),_loadSituation(0),_mutex(nullptr) {}
        ServerHost(string ip, uint16_t prot, size_t loadSituation, mutex* mtx)
            :_ip(ip),_prot(prot),_loadSituation(loadSituation),_mutex(mtx) {}
        ~ServerHost() {}
    public:
        size_t GetLoadSituation() { 
            size_t loadSituation = 0;
            if(_mutex != nullptr) {
                unique_lock<mutex> lock(*_mutex);
                loadSituation = _loadSituation; 
            }
            return loadSituation;
        }
        void IncreaseLoad() {
            if(_mutex != nullptr) {
                unique_lock<mutex> lock(*_mutex);
                ++_loadSituation;
            }
        }
        void ReduceLoad() {
            if(_mutex != nullptr) {
                unique_lock<mutex> lock(*_mutex);
                --_loadSituation;
            }
        }
        void ClearLoad() {
            if(_mutex != nullptr) {
                unique_lock<mutex> lock(*_mutex);
                _loadSituation = 0;
            }
        }
        string GetIP() { return _ip; }
        uint16_t GetPort() {return _prot; }

    private:
        string _ip;
        uint16_t _prot;
        size_t _loadSituation;
        mutex* _mutex;
    };

    

    //负载均衡模块
    const string ServerHostConfFilePath = "./conf/ServerHost.conf";
    class LoadBalancer
    {
    public:
        LoadBalancer() { 
            assert(LoadFromConf(ServerHostConfFilePath)); 
            LOG(NORMAL) << "Successfully loaded configuration file: " << ServerHostConfFilePath << endl;
        }
        ~LoadBalancer() {}

    public:
        bool SelectServiceHost(size_t* hostID, ServerHost** serverHost) //选择服务主机
        {
            unique_lock<mutex> lock(_mutex);

            int onlineNumber = _onlineServerHosts.size();
            if(onlineNumber == 0) {
                LOG(FATAL) << "All service hosts providing code processing are offline" << endl;
                return false;
            } 

            size_t minLoadSituation = (_serverHosts[_onlineServerHosts[0]].GetLoadSituation());
            *hostID = _onlineServerHosts[0];
            *serverHost = &_serverHosts[_onlineServerHosts[0]];
            for(int i = 0; i < onlineNumber; ++i) {
                if(minLoadSituation > _serverHosts[_onlineServerHosts[i]].GetLoadSituation()) {
                    minLoadSituation = _serverHosts[_onlineServerHosts[i]].GetLoadSituation();
                    *hostID = _onlineServerHosts[i];
                    *serverHost = &_serverHosts[_onlineServerHosts[i]];
                }
            }
            return true;
        }

        void OnlineServerHost()
        {
            unique_lock<mutex> lock(_mutex);
            for(size_t i = 0; i < _offlineServerHosts.size(); ++i) 
                _onlineServerHosts.push_back(_offlineServerHosts[i]);
            _offlineServerHosts.clear();
            LOG(NORMAL) << "All hosts are online" << endl;
        }
        void OfflineServerHost(size_t hostID)
        {
            unique_lock<mutex> lock(_mutex);
            for(size_t i = 0; i < _onlineServerHosts.size(); ++i) {
                if(_onlineServerHosts[i] == hostID) {
                    _serverHosts[hostID].ClearLoad();
                    _onlineServerHosts.erase(_onlineServerHosts.begin() + i);
                    _offlineServerHosts.push_back(hostID);
                    break;
                }
            }
        }

    public://for Debug
        void ShowServerHost() {
            unique_lock<mutex> lock(_mutex);
            std::cout << "当前在线主机列表" << endl;
            for(auto& id : _onlineServerHosts) {
                std::cout << id << " ";
            }
            std::cout << endl;
            std::cout << "当前离线主机列表" << endl;
            for(auto& id : _offlineServerHosts) {
                std::cout << id << " ";
            }
            std::cout << endl;
        }

    private:
        bool LoadFromConf(const string& confFilePath) 
        {
            ifstream input(confFilePath);
            if(!input.is_open()) {
                LOG(FATAL) << "Failed to open host configuration file: " << confFilePath << endl;
                return false;
            }

            string lineMessage;
            while(getline(input, lineMessage)) 
            {
                vector<string> temp = SplitTool::SplitString(lineMessage, ":");
                if(temp.size() != 2) {
                    LOG(WARING) << "Failed to obtain host information(sharding): " << lineMessage << endl;
                    continue;
                }
                ServerHost serverHost(temp[0], atoi(temp[1].c_str()), 0, new mutex);
                _onlineServerHosts.push_back(_serverHosts.size());//默认在线
                _serverHosts.push_back(serverHost);
            }

            input.close();
            return true;
        }

    private:
        vector<ServerHost> _serverHosts;//数据来自配置文件, 启动后不再发生改变
        vector<size_t> _onlineServerHosts;//存储主机ID, 利用在_serverHosts中的下标作为标识主机的ID
        vector<size_t> _offlineServerHosts;
        mutex _mutex;
    };
}