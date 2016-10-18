//
// Created by serg on 10/18/16.
//

#include "Db.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include <iostream>

const size_t INITIAL_POOL_SIZE = 8;

Db::Db(std::string host, std::string user, std::string password)
        : _connection_pool(INITIAL_POOL_SIZE) {
    try {
        auto driver = sql::mysql::get_mysql_driver_instance();

        for (auto c: _connection_pool) {
            c.isInUse = false;
            c.connection = driver->connect(host, user, password);
        }
    } catch (sql::SQLException &e) {
        std::cout << "Exception while creating Db object" << std::endl;
        std::cout << "Err:" << e.what() << " (MySQL error code:" << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

        exit(1);
    }
}

