#ifndef SQLHELPER_H
#define SQLHELPER_H
#include "sqlite3.h"
#include <iostream>
#include <list>
#include "transport.h"
#include <vector>
using namespace std;
class sqlHelper
{
public:
    sqlHelper();
    ~sqlHelper();
    int open();
    void insert_table(string table,list<string> name,list<string> value);
    string select_table(string sql);
    void delete_table(string table, string arg);
    void reset();
    void clear_table(string table);
	void update_table(string table,list<string> name,list<string> value);
	void exec(string sql);
    std::vector <RECTSET> getRect(int *len,bool isset);
    WINDOW getWindow();
    float getSerialTemp();
    void recovery(bool clear_window);
private:
    sqlite3 *db;
    list<string> ret;
    void create_table();



};

#endif // SQLHELPER_H
