#include "HttpCookie.h"

HttpCookie::HttpCookie(string name, string value)
{
    sprintf(buf, "%s=%s", name.c_str(), value.c_str());
    cookie = string(buf);
}

void HttpCookie::setExpires(time_t expire_time)
{
    char tbuf[128];

    strftime(tbuf, sizeof(tbuf),"%a, %d %b %Y %H:%M:%S GMT",gmtime(&expire_time));
    sprintf(buf,"%s; %s=%s", cookie.c_str(), "expires", tbuf);
    cookie = string(buf);
}

void HttpCookie::setPath(string path)
{
    sprintf(buf,"%s; %s=%s", cookie.c_str(), "path", path.c_str());
    cookie = string(buf);
}

string HttpCookie::getCookie()
{
    return cookie;
}