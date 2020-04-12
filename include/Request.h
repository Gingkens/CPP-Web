#pragma once

#include <iostream>
#include <vector>
#include <map>

using namespace std;


class Request
{
public:
    Request(string start_line, vector<string> headers);
    string getStartLine();
    string operator [](string name){return headers[name];}
    void setContent(string content);
    string getMethod(){return method;}
    string getUri(){return uri;}
    string getFilename(){return filename;}
    string getFiletype(){return filetype;}
    string getData(string name){return datas[name];}
    string getData(){return entity;}
    string getArgs(){return cgi_args;}
    bool Is_static(){return is_static;}
private:
    void parse_uri();
    void parse_filetype();
    //vector<pair<string, string>> headers;
    map<string, string> headers;
    map<string, string> datas;              // 请求附带的数据
    string entity, start_line;
    string method, uri, version;
    string filename, filetype, cgi_args;
    bool is_static;    
};