#include "DB.h"

DB::DB()
{
    conn = mysqlpp::Connection(false);

    if(!conn.connect("Web", "127.0.0.1", "root", "root", 3306 ))
    {
        fprintf(stderr, "failed to connect DB:%s\n", conn.error());
        exit(1);
    }
}

mysqlpp::StoreQueryResult DB::query(string table, string predicate)
{
    string sql = "select * from " + table;
    if(!predicate.empty()) sql += " where " + predicate;

    mysqlpp::Query query = conn.query(sql);
    mysqlpp::StoreQueryResult res = query.store();
    if(res) return res;
    else
    {
        fprintf(stderr, "Failed to connect the %s table: %s\n", table.c_str(), query.error());
        exit(1);
    }
}

bool DB::find(string table, string predicate)
{
    mysqlpp::StoreQueryResult res = query(table, predicate);
    if(res.empty()) return false;
    return true;
}