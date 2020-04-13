#include "session.h"
#include <fstream>
#include <sstream>

Session::Session(string cookie)
{
    auto equal = cookie.find('=');
    auto colon = cookie.find(';');

    while(equal != cookie.npos && colon != cookie.npos)
    {
        cookies[ cookie.substr(0, equal) ] = cookie.substr(equal+1, colon-equal-1);
        // std::cout <<"cookie:"<< cookie.substr(0, equal) << "=" << cookie.substr(equal+1, colon-equal-1) \
        // <<", length:"<< cookie.substr(equal+1, colon-equal-1).size() << '\n';
        fprintf(stderr, "%s=%s; ", cookie.substr(0, equal).c_str(), \
            cookie.substr(equal+1, colon-equal-1).c_str());
        
        if(colon == cookie.npos) break;

        cookie = cookie.substr(colon+2);

        equal = cookie.find('=');
        colon = cookie.find(';');
    }

    if(equal != cookie.npos)
    {
        colon = cookie.find_last_of('\r');
        cookies[cookie.substr(0, equal)] = cookie.substr(equal+1, colon-equal-1);  

                fprintf(stderr, "%s=%s; ", cookie.substr(0, equal).c_str(), \
            cookie.substr(equal+1, colon-equal-1).c_str());
        //     std::cout <<"cookie:"<< cookie.substr(0, equal) << "=" << cookie.substr(equal+1, colon-equal-1) \
        // <<", length:"<< cookie.substr(equal+1, colon-equal-1).size() << '\n';
    }
    fprintf(stderr, "\n\n");
}
