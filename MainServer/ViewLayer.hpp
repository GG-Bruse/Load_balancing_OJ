#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctemplate/template.h>
#include "ModelLayer.hpp"

using std::vector;
using std::string;
using namespace ctemplate;
using ModelLayer::Question;
using ModelLayer::Job;

namespace ViewLayer
{
    const string ctemplateHtmlPath = "./wwwroot/ctemplateHtml/";
    class View
    {
    public:
        void AllQuestionsRomanceHtml(const vector<Question>& allQuestions, string* html/*输入输出型参数*/) 
        {
            string srcHtmlPath = ctemplateHtmlPath + "AllQuestions.html";

            TemplateDictionary father("AllQuestions");//数据字典
            for(size_t i = 0; i < allQuestions.size(); ++i) 
            {
                TemplateDictionary* child = father.AddSectionDictionary("question_list");//子字典
                child->SetValue("number", allQuestions[i]._number);
                child->SetValue("title", allQuestions[i]._title);
                child->SetValue("difficulty", allQuestions[i]._difficulty);
            }

            //获取被渲染网页
            Template* templatePtr = Template::GetTemplate(srcHtmlPath, DO_NOT_STRIP);
            //进行渲染
            templatePtr->Expand(html, &father);
        }

        void DesignatedQuestionRomanceHtml(const Question& question, string* html/*输入输出型参数*/) 
        {
            string srcHtmlPath = ctemplateHtmlPath + "DesignatedQuestion.html";
            //构建字典
            TemplateDictionary obj("DesignatedQuestion");
            obj.SetValue("number", question._number);
            obj.SetValue("title", question._title);
            obj.SetValue("difficulty", question._difficulty);
            obj.SetValue("describe", question._describe);
            obj.SetValue("headContent", question._headContent);
            //获取被渲染的网页
            Template* templatePtr = Template::GetTemplate(srcHtmlPath, DO_NOT_STRIP);
            //进行渲染
            templatePtr->Expand(html, &obj);
        }

        void AllJobRomanceHtml(const vector<Job>& allJob, string* html) {
            string srcHtmlPath = ctemplateHtmlPath + "AllJobs.html";

            TemplateDictionary father("AllJobs");//数据字典
            for(size_t i = 0; i < allJob.size(); ++i) 
            {
                TemplateDictionary* child = father.AddSectionDictionary("joblist");//子字典
                child->SetValue("number", allJob[i]._number);
                child->SetValue("name", allJob[i]._name);
                child->SetValue("position", allJob[i]._position);
                child->SetValue("salary", allJob[i]._salary);
            }

            Template* templatePtr = Template::GetTemplate(srcHtmlPath, DO_NOT_STRIP);//获取被渲染网页
            templatePtr->Expand(html, &father);//进行渲染
        }

        void DesignatedJobRomanceHtml(const Job& job, string* html/*输入输出型参数*/) 
        {
            string srcHtmlPath = ctemplateHtmlPath + "DesignatedJob.html";
            //构建字典
            TemplateDictionary obj("DesignatedQuestion");
            obj.SetValue("name", job._name);
            obj.SetValue("position", job._position);
            obj.SetValue("salary", job._salary);
            obj.SetValue("address", job._address);
            obj.SetValue("describe", job._describe);
            obj.SetValue("phone", job._phone);
            //获取被渲染的网页
            Template* templatePtr = Template::GetTemplate(srcHtmlPath, DO_NOT_STRIP);
            //进行渲染
            templatePtr->Expand(html, &obj);
        }
    };
}