#ifndef SQLBLOCKCHAIN_H
#define SQLBLOCKCHAIN_H

#include "sqlite3.h"
#include "blockchaindata.h"
class sqlblockchain
{
public:
    sqlblockchain();
    int insert_block_chain(blockchainData *data);
    string getLastHash();
private:
    sqlite3 *db;
    int open();
    void create_table();
};

#endif // SQLBLOCKCHAIN_H
