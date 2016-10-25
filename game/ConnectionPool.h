//
// Created by serg on 10/24/16.
//

#ifndef TIC_TAC_TOE_CONNECTIONPOOL_H
#define TIC_TAC_TOE_CONNECTIONPOOL_H

#include <string>
#include <memory>
#include <set>
#include <vector>
#include <thread>
#include <sstream>
#include <map>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace sql {
    class Connection;
};

struct ConnectionWithInfo {

    sql::Connection *connection;
    bool isInUse;
};

class ConnectionPool {
public:
    ConnectionPool(std::string host, std::string user, std::string password, std::string database, size_t capacity = 4);

    std::shared_ptr<sql::Connection> getConnection();
private:
    std::vector<ConnectionWithInfo> _connection_pool;

    std::mutex _mutex;
    std::condition_variable _connection_notify;

    std::string _host;
    std::string _user;
    std::string _password;
    std::string _database;
    std::string _capacity;
};


#endif //TIC_TAC_TOE_CONNECTIONPOOL_H
