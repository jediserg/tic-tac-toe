//
// Created by serg on 10/24/16.
//

#ifndef TIC_TAC_TOE_READER_H
#define TIC_TAC_TOE_READER_H

#include <string>
#include <memory>
#include <set>
#include <vector>
#include <map>

class ConnectionPool;

class DbStore {
public:
    DbStore(std::string host, std::string user, std::string password, std::string database,
            std::string pool_size);

    std::map<std::string, std::string>
    loadData(std::string table, std::string id_field, std::vector<std::string> &fields);

    void saveData(std::string table_name, const std::map<std::string, std::string> &data);

private:
    std::unique_ptr<ConnectionPool> _pool;
    std::string _host;
    std::string _user;
    std::string _password;
    std::string _database;
    std::string _pool_size;
};


#endif //TIC_TAC_TOE_READER_H
