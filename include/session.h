#pragma once

#include "config.h"

#include <iostream>
#include <map>

using namespace std;

class Session
{
public:
    Session(string cookie);
    string operator[] (string name){return cookies[name];}
    bool find(string name){return cookies.find(name) != cookies.end();}
private:
    map<string, string> cookies;
    string sessionFile = SESSION_FILE;
};