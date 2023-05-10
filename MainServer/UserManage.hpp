#pragma once
#include <iostream>
#include <vector>
#include <mutex>

using std::unique_lock;
using std::vector;
using std::mutex;


namespace usermanage
{
    class UserManage
    {
    public:
        UserManage(){}
        ~UserManage() {}
    public:
        //当用户登录之后，将登录的用户保存
        void AddUserToSet(int userId, int flag = 0) {
            unique_lock<mutex> lock(_mutex);
            if (flag == 0) _userVector_Administrator.push_back(userId);
            else _userVector_Ordinary.push_back(userId);
        }

    public:
        vector<int> _userVector_Administrator;//管理员
        vector<int> _userVector_Ordinary;//普通用户
        mutex _mutex;
    };
}
