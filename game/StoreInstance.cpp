//
// Created by serg on 10/26/16.
//
#include "StoreInstance.h"

MysqlStore& getMysqlStore(std::string host, std::string user, std::string password, std::string database, int pool_size)
{
    static MysqlStore store(host, user, password, database, pool_size);
    return store;
}
