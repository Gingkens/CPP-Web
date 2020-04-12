#include "Response.h"
#include <string.h>

Response::Response(){}

Response::Response(int status, string msg)
{
    sprintf(buf, "%s %s %s\r\n", VERSION, to_string(status).c_str(), msg.c_str());
//    start_line = string(buf);
    response = string(buf);
}

Response::Response(int status, string msg, string filetype, int cLength):Response(status, msg)
{
    Content_Type(filetype);
    Content_Length(cLength);
}

void Response::init(int status, string msg, string filename, string filetype, int cLength)
{
    *this  = Response(status, msg, filetype, cLength);
    response_file = filename;
}

void Response::Redirect(string location)
{
    init(302, "FOUND", location, "text/html", 0);
    sprintf(buf, "Location: %s\r\n", location.c_str());
    response += string(buf);
}

void Response::Content_Type(string type)
{
    sprintf(buf, "Content-Type: %s\r\n", type.c_str());
//    headers += string(buf);
    response += string(buf);
}

void Response::Content_Length(int len)
{
    sprintf(buf, "Content-Length: %d\r\n", len);
//    headers += string(buf);
    response += string(buf);
}

void Response::addCookie(HttpCookie cookie)
{
    sprintf(buf, "Set-Cookie: %s\r\n", cookie.getCookie().c_str());
//    headers += string(buf);
    response += string(buf);
}

string Response::getResponse(bool verbose)
{
    if(verbose)
    {
        std::cout << "Response headers:\n";
        std::cout << response << '\n';
        cout << "--------------------------------------------------\n";

    }
    return string(response + "\r\n");
}

size_t Response::size()const
{
    return string(response + "\r\n").size();
}
