//
// Created by serg on 10/18/16.
//

#ifndef TIC_TAC_TOE_DB_H
#define TIC_TAC_TOE_DB_H

#include <string>
#include <memory>
#include <set>
#include <vector>
#include <thread>

namespace sql {
    class Connection;
};

struct ConnectionWithInfo {

    sql::Connection *connection;
    bool isInUse;
};

class Db {
public:
    Db(std::string host, std::string user, std::string password);

private:
    std::vector<ConnectionWithInfo> _connection_pool;
};


#endif //TIC_TAC_TOE_DB_H
