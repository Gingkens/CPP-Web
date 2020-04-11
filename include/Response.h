#pragma once

#include "HttpCookie.h"
#include <iostream>

#define VERSION "HTTP/1.1"
#define DEBUG true

using namespace std;

class Response
{
public:
    Response();
    Response(int status, string msg, string filetype, int cLength);
    Response(int status, string msg);
    size_t size() const;
    void init(int status, string msg, string filename, string filetype, int cLength);
    void Content_Type(string type);
    void Content_Length(int len);
    void Redirect(string location);
    void addCookie(HttpCookie);
    string getResponse();
    string getResponseFile(){return response_file;}
private:
    string start_line, headers, response;
    string response_file;
    char buf[1024];
};