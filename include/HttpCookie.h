#pragma once

#include "config.h"
#include <iostream>
using namespace std;

class HttpCookie
{
public:
    HttpCookie(string name, string value);
    void setExpires(time_t expire_time = time(nullptr) + SESSION_EXPIRES);
    void setPath(string);
    string getCookie();
private:
    string cookie;
    char buf[1024];
};