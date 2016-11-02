//
// Created by serg on 10/28/16.
//

#ifndef TIC_TAC_TOE_DBFIXTURE_H
#define TIC_TAC_TOE_DBFIXTURE_H

#include <gtest/gtest.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>

class DbFixture : public ::testing::Test {
public:
    static constexpr const char *HOST = "localhost";
    static constexpr const char *USER = "root";
    static constexpr const char *PASSWORD = "root";
    static constexpr const char *DB = "test-game-db";

    DbFixture() {
        auto driver = sql::mysql::get_mysql_driver_instance();
        auto con = driver->connect(HOST, USER, PASSWORD);
        auto stmt = con->createStatement();

        stmt->execute(std::string("DROP DATABASE IF EXISTS `") + DB + "`");
        stmt->execute(std::string("CREATE DATABASE IF NOT EXISTS `") + DB + "`");

        delete stmt;
        delete con;

        std::cout << "Test db created" << std::endl;
    }

    ~DbFixture() {
        auto driver = sql::mysql::get_mysql_driver_instance();
        auto con = driver->connect(HOST, USER, PASSWORD);
        auto stmt = con->createStatement();

        stmt->execute(std::string("DROP DATABASE `") + DB + "`");

        delete stmt;
        delete con;

        std::cout << "Test db droped" << std::endl;
    }

private:
};


#endif //TIC_TAC_TOE_DBFIXTURE_H
