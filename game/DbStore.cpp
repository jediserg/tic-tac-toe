//
// Created by serg on 10/24/16.
//

#include <map>
#include <vector>
#include "DbStore.h"
#include "ConnectionPool.h"

std::map<std::string, std::string>
DbStore::loadData(std::string table, std::string id_field, std::vector<std::string> &fields) {
    return std::map<std::string, std::string>();
}

void DbStore::saveData(std::string table_name, const std::map<std::string, std::string> &data) {
    auto connection = _pool->getConnection();

}

DbStore::DbStore(std::string host, std::string user, std::string password, std::string database,
                 std::string pool_size)
        : _host(host), _user(user), _password(password), _database(database), _pool_size(pool_size) {}
