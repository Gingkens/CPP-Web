#include "Request.h"

#include <sstream>
#include <string.h>

Request::Request(string start_line, vector<string> headers):start_line(start_line)
{
    istringstream is(start_line);
    is >> method >> uri >> version;
    string name, value;
    cout << "--------------------------------------------------\n";

    cout << "Request Headers:\n";
    cout << start_line;

    for(auto header:headers)
    {
        auto p = header.find(':');
        name = header.substr(0, p);
        value = header.substr(p+2);
        cout << name << ":" << value;
        this->headers[name] = value;
    }
    cout << "\r\n";
    parse_uri();

}

void Request::parse_uri()
{
    filename = ".";
    is_static = true;
    if(uri == "/")
    {
        is_static = false;
        filename = "./cgi-bin/index";  //根目录
    } 
    else if(uri.find("cgi-bin") == uri.npos)  //静态文件
    {
        filename += uri;
    }
    else            //动态文件
    {
        is_static = false;
        auto p = uri.find('?');
        filename += uri.substr(0, p);
        cgi_args = uri.substr(p+1);
    }
    parse_filetype();
}

void Request::parse_filetype()
{
    if(strstr(filename.c_str(), ".html"))
        filetype = "text/html";
    else if((strstr(filename.c_str(), ".gif")))
        filetype = "image/gif";
    else if(strstr(filename.c_str(), ".png"))
        filetype = "image/png";
    else if(strstr(filename.c_str(), ".jpg"))
        filetype = "image/jpeg";
    else if(strstr(filename.c_str(), ".css"))
        filetype = "text/css";
    else if(strstr(filename.c_str(), ".ico"))
        filetype = "image/x-icon";
    else
        filetype = "text/plain";
}

void Request::setContent(string query)
{
    if(query.empty()) return;

    entity = query;
    auto p1 = query.find('=');
    auto p2 = query.find('&');

    while(p2 != query.npos)
    {
        datas[query.substr(0, p1)] = query.substr(p1 + 1, p2-p1-1);
        query = query.substr(p2+1);
        p1 = query.find('=');
        p2 = query.find('&');
    }
    
 //   cout << query.substr(0, p1) << ":" << query.substr(p1 + 1, p2-p1-1) << endl;

    datas[query.substr(0, p1)] = query.substr(p1 + 1, p2-p1-1);

    if(!entity.empty())
    {
        cout << "Entity-body:\n";
        cout << entity << "\r\n\r\n";
    }

}

string Request::getStartLine()
{
    return start_line;
}