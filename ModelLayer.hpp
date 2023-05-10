#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include "../Common/ComponentTool.hpp"
#include "../Common/LogMessage.hpp"

using std::string;
using std::vector;
using std::unordered_map;
using std::ifstream;
using std::endl;
using namespace ComponentTool;
using namespace DailyRecord;

namespace ModelLayer 
{
    struct Question {
        string _number;
        string _title;
        string _difficulty;
        int _cpuLimit;
        int _memoryLimit;
        string _describe;
        string _headContent;
        string _tailContent;
    };

    const string questionListPath = "./Questions/questions.list";
    const string questionPath = "./Questions/";

    class Model
    {
    public:
        Model() { assert(LoadQuestionList(questionListPath)); }
        ~Model() {}

    public:
        bool ObtainAllQuestions (vector<Question>* allQuetions) {
            if(_questions.size() == 0) {
                LOG(ERROR) << "User failed to obtain question bank" << endl;
                return false;
            }
            for(unordered_map<string,Question>::iterator it = _questions.begin(); it != _questions.end(); ++it) 
                allQuetions->push_back(it->second);
            return true;
        }
        bool ObtainDesignatedQuestion(const string& number, Question* question) {
            unordered_map<string,Question>::iterator it = _questions.find(number);
            if(it == _questions.end()) {
                LOG(ERROR) << "User failed to obtain the question, number:" << number << endl;
                return false;
            }
            (*question) = (it->second);
            return true;
        }

    private:
        bool LoadQuestionList(const string& questionListPath) {
            ifstream input(questionListPath);
            if(!input.is_open()) {
                LOG(FATAL) << "Failed to load the question bank. Please check if the file exists" << endl;
                return false;
            }

            string lineMeaasge;
            while(getline(input, lineMeaasge))
            {   
                vector<string> ret = SplitTool::SplitString(lineMeaasge, " ");
                if(ret.size() != 5) {
                    LOG(WARNING) << "Failed to load the question, please check the file format" << endl;
                    continue;
                }
                Question question;
                question._number = ret[0];
                question._title = ret[1];
                question._difficulty = ret[2];
                question._cpuLimit = atoi(ret[3].c_str());
                question._memoryLimit = atoi(ret[4].c_str());

                string targetQuestionPath = questionPath + question._number + "/";
                FileTool::ReadFromFile(targetQuestionPath + "Describe.txt", &question._describe, true);
                FileTool::ReadFromFile(targetQuestionPath + "Head.cpp", &question._headContent, true);
                FileTool::ReadFromFile(targetQuestionPath + "Tail.cpp", &question._tailContent, true);

                _questions[question._number] = question;
            }
            LOG(NORMAL) << "Successfully loaded the question bank" << endl;
            input.close();
            return true;
        }

    private:
        unordered_map<string, Question> _questions;
    };
}  
