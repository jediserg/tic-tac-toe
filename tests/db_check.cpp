//
// Created by serg on 10/23/16.
//

#include <gtest/gtest.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>

#include "Db.h"

class DbFixture : public ::testing::Test
{
public:
    static constexpr const char *HOST = "localhost";
    static constexpr const char *USER = "root";
    static constexpr const char *PASSWORD = "testroot";
    static constexpr const char *DB = "test-game-db";

    DbFixture()
    {
        auto driver = sql::mysql::get_mysql_driver_instance();
        auto con = driver->connect(HOST, USER, PASSWORD);
        auto stmt = con->createStatement();

        stmt->execute(std::string("CREATE DATABASE IF NOT EXISTS `") + DB + "`");

        delete stmt;
        delete con;

        std::cout << "Test db created" << std::endl;
    }

    ~DbFixture()
    {
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

class TestClass
{
public:
    TestClass(std::map<std::string, std::string> data)
    {
        field1 = data["field1"];
        field2 = data["field2"];
    }

    std::string getField1()
    {
        return field1;
    }

    std::string getField2()
    {
        return field2;
    }

private:
    std::string field1;
    std::string field2;
};

TEST_F(DbFixture, CheckSaveLoad)
{
    Db db("localhost", "root", "testroot");
    db.addSchema<TestClass>("test_class", "field1", {{"field1", &TestClass::getField1},
                                                     {"field2", &TestClass::getField2}});

    TestClass obj{{{"field1", "value1"}, {"field2", "value2"}}};

    db.save(obj);

    std::shared_ptr<TestClass> obj1 = db.load<TestClass>("value1");
}
