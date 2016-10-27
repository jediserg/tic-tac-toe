//
// Created by serg on 10/23/16.
//

#include <gtest/gtest.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_driver.h>
#include <Validatable.h>
#include <MysqlStorage.h>
#include "FakeStore.h"
#include "StoreInstance.h"

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

class TestClass : public Validatable {
public:
    TestClass(std::map<std::string, std::string> data) {
        field1 = data["field1"];
        field2 = data["field2"];
        try {
            int_field = std::stoi(data["int_field"]);
        } catch (...) {
            setValid(false);
        }
    }

    std::string getField1() {
        return field1;
    }

    std::string getField2() {
        return field2;
    }

    int getIntField() {
        return int_field;
    }

private:
    std::string field1;
    std::string field2;

    int int_field;
};

TEST_F(DbFixture, CheckSaveLoad) {
    Store<MysqlStorage> db(HOST, USER, PASSWORD, DB, 4);

    db.addTable<TestClass>("test_class", "field1", {
            {"field1",    &TestClass::getField1},
            {"field2",    &TestClass::getField2},
            {"int_field", [](TestClass *obj) { return std::to_string(obj->getIntField()); }}
    });

    db.createTable<TestClass>();

    TestClass obj{{{"field1", "value1"}, {"field2", "value2"}, {"int_field", "42"}}};
    db.save(obj);

    auto obj1 = db.load<TestClass>("value1");

    EXPECT_EQ("value1", obj1->getField1());
    EXPECT_EQ("value2", obj1->getField2());
    EXPECT_EQ(42, obj1->getIntField());

    db.dropTable<TestClass>();
}

TEST(CheckStore, CheckSaveLoadFromFakeDb) {
    Store<FakeStore> db;

    db.addTable<TestClass>("test_class", "field1", {
            {"field1",    &TestClass::getField1},
            {"field2",    &TestClass::getField2},
            {"int_field", [](TestClass *obj) { return std::to_string(obj->getIntField()); }}
    });

    TestClass obj{{{"field1", "value1"}, {"field2", "value2"}, {"int_field", "42"}}};
    db.save(obj);

    auto obj1 = db.load<TestClass>("value1");
    EXPECT_EQ("value1", obj1->getField1());
    EXPECT_EQ("value2", obj1->getField2());
    EXPECT_EQ(42, obj1->getIntField());
}

TEST_F(DbFixture, CheckGettingStoreInstance) {
    auto &store1 = getMysqlStore(HOST, USER, PASSWORD, DB, 4);
    auto &store2 = getMysqlStore();
    auto &store3 = getMysqlStore();

    EXPECT_EQ(&store1, &store2);
    EXPECT_EQ(&store1, &store3);
}