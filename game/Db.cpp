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

Db::Db(std::string host, std::string user, std::string password, size_t capacity)
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

sql::Connection *Db::getConnection()
{
    std::unique_lock<std::mutex> lock(_mutex);
    sql::Connection *free_connection = nullptr;

    while(free_connection == nullptr)
    {
        for(auto &c: _connection_pool)
        {
            if(!c.isInUse)
            {
                free_connection = c.connection;
                c.isInUse = true;
                break;
            }
        }

        if(free_connection == nullptr)
            _connection_notify.wait(_mutex);
    }
    return nullptr;
}

std::map<std::string, std::string>
Db::_loadData(std::string table, std::string id_field, std::vector<std::string> &fields)
{
    return std::map<std::string, std::string>();
}

void Db::_saveData(std::string table_name, std::map<std::string, std::string> data)
{
    auto con = getConnection();
}

void Db::releaseConnection(sql::Connection *c)
{
    for(auto &it: _connection_pool)
    {
        if(it.connection == c)
        {
            it.isInUse = false;
        }
    }

    _connection_notify.notify_one();
}

