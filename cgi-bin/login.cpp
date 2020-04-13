#include <iostream>
#include "DB.h"
#include "Helper.h"
#include "Response.h"
#include "HttpCookie.h"

using namespace std;

int main(int argc, char *argv[])
{
    char sql[128];
    DB db;
    Response response;

    auto infos = parseData(argv[3]);

    sprintf(sql, "username='%s' and password='%s'", infos["name"].c_str(), infos["password"].c_str());

    response.Redirect("/");
    HttpCookie cookie("session", infos["name"]);
    cookie.setPath("/");

    if(db.find("user", sql))
    {
        fprintf(stderr, "success find user\n");
        cookie.setExpires();
    }
    else
    {
        fprintf(stderr, "cannot find user\n");
        cookie.setExpires(time(nullptr));
    }

    response.addCookie(cookie);
    cout << response.getResponse(false);

    fprintf(stderr, "cgi-bin:Response headers:\n%s", response.getResponse(false).c_str());

    
    return 0;
}
