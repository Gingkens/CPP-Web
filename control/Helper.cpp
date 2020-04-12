#include "Helper.h"

map<string, string> parseData(string query)
{
    map<string, string> datas;
    auto p1 = query.find('=');
    auto p2 = query.find('&');

    while(p2 != query.npos)
    {
        datas[query.substr(0, p1)] = query.substr(p1 + 1, p2-p1-1);
        query = query.substr(p2+1);
        p1 = query.find('=');
        p2 = query.find('&');
    }
    
    datas[query.substr(0, p1)] = query.substr(p1 + 1, p2-p1-1);

    return datas;
}