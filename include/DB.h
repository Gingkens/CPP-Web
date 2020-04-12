#pragma once

#include <mysql++/mysql++.h>
#include <iostream>

using namespace std;

class DB
{
public:
    DB();
    mysqlpp::StoreQueryResult query(string table, string predicate = "");
    bool find(string table, string predicate = "");
private:
    mysqlpp::Connection conn;
};