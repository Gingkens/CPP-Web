#include "session.h"
#include "Response.h"
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
    ostringstream headers;
    ostringstream msg_entity;       //消息实体
    Session sess(argv[2]);

//    RIO rio(1);         // 标准输出文件描述符 1， 已重定向
//    Response response;

    // msg_entity << argc << '\n';
    // for(int i=0;i<argc;++i)
    //     msg_entity << argv[i] << '\n';

    msg_entity << "<!DOCTYPE html>\n";
    msg_entity << "<html>\n";
    msg_entity << "<head>\n";
    msg_entity << "<title>Tiny Web Server</title>\n";
    msg_entity << "<meta  charset='utf-8'>\n";
    msg_entity << "<link rel = 'stylesheet' href='/bootstrap/css/bootstrap.min.css'>\n";
    msg_entity << "<script src='myScript.js'></script>\n";
    msg_entity << "</head>\n";
    msg_entity << "<body>\n";
    msg_entity << "<div class='navbar navbar-inverse' role='navigation'>\n";
    msg_entity << "<div class='container'>\n";
    msg_entity << "<div class='navbar-header'>\n";
    msg_entity << "<a class='navbar-brand' href='/'>Tiny Web</a>\n";
    msg_entity << "</div>\n";
    msg_entity << "<div class='navbar-collapse collapse'>\n";
    msg_entity << "<ul class='nav navbar-nav'>\n";
    msg_entity << "<li><a href='/'>Home</a></li>\n";
    msg_entity << "</ul>\n";
    msg_entity << "</div>\n";
    msg_entity << "</div>\n";
    msg_entity << "</div>\n";
    msg_entity << "<div class='container'>\n";

    msg_entity << "<div class='page-header'>\n";
    if( sess.find("session") )
        msg_entity <<"<h1>Hello, " << sess["session"]  << "!</h1>\n";
    else
        msg_entity <<"<h1>Hello, Stranger!</h1>\n"; 
    msg_entity << "</div>\n";

    msg_entity << "<form method='post' action='/cgi-bin/index' name='Login' onsubmit='return validateForm()'>\n";
    msg_entity << "<div class='form-group'>\n";
    msg_entity << "<label for='exampleInputEmail1'>What is your name?</label>\n";
    msg_entity << "<input class='form-control' type='text' name='name' placeholder='Name'>\n";
    msg_entity << "<label for='exampleInputEmail1'>Password</label>\n";
    msg_entity << "<input class='form-control' type='password' name='password' placeholder='Password'>\n";
    msg_entity << "</div>\n";
    msg_entity << "<button type='submit' class='btn btn-default'>Submit</button>\n";
    msg_entity << "</form>\n";
    msg_entity << "</div>\n";
    msg_entity << "</body>\n";
    msg_entity << "</html>\n";


    Response response(200, "OK", "text/html", msg_entity.str().size());

    cout << response.getResponse(false);

    fprintf(stderr, "cgi-bin:Response headers:\n%s", headers.str().c_str());


    cout << msg_entity.str().c_str();


    return 0;
}

