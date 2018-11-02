#ifndef SQLHELPER_H
#define SQLHELPER_H
#include "sqlite3.h"
#include <iostream>
#include <list>
using namespace std;
class sqlHelper
{
public:
    sqlHelper();
    ~sqlHelper();
    int open();
    void insert_table(string table,list<string> name,list<string> value);
    list<string> select_table(string sql);
    void delete_table(string table, string arg);
    void release();
    void clear_table(string table);
private:
    sqlite3 *db;
    list<string> ret;
    void create_table();



};

#endif // SQLHELPER_H
