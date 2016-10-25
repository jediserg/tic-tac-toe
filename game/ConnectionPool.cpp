//
// Created by serg on 10/24/16.
//

#include "ConnectionPool.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>

ConnectionPool::ConnectionPool(std::string host, std::string user, std::string password, std::string database,
                               size_t capacity)
        : _host(host), _user(user), _password(password), _database(database), _connection_pool(capacity) {
    try {
        auto driver = sql::mysql::get_mysql_driver_instance();

        for (auto &c: _connection_pool) {
            c.isInUse = false;
            c.connection = driver->connect(_host, _user, _password);
            c.connection->setSchema(database);
        }
    } catch (sql::SQLException &e) {
        std::cout << "Exception while creating Db object" << std::endl;
        std::cout << "Err:" << e.what() << " (MySQL error code:" << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

        exit(1);
    }
}

std::shared_ptr<sql::Connection> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(_mutex);
    sql::Connection *free_connection = nullptr;

    while (free_connection == nullptr) {
        for (auto &c: _connection_pool) {
            if (!c.isInUse) {
                free_connection = c.connection;
                c.isInUse = true;
                break;
            }
        }

        if (free_connection == nullptr)
            _connection_notify.wait(lock);
    }

    if (free_connection->isValid())
        return std::shared_ptr<sql::Connection>(free_connection, [this](sql::Connection *con) {
            std::unique_lock<std::mutex> lock(_mutex);

            for (auto &c: _connection_pool) {
                if (c.connection == con) {
                    c.isInUse = false;
                    _connection_notify.notify_one();
                }
            }
        });
}
