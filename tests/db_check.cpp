//
// Created by serg on 10/23/16.
//

#include <gtest/gtest.h>

#include <Validatable.h>
#include <MysqlStorage.h>
#include "FakeStore.h"
#include "StoreInstance.h"
#include "DbFixture.h"


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

    std::string getField1() const{
        return field1;
    }

    std::string getField2() const {
        return field2;
    }

    int getIntField() const {
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
            {"int_field", [](const TestClass *obj) { return std::to_string(obj->getIntField()); }}
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
            {"int_field", [](const TestClass *obj) { return std::to_string(obj->getIntField()); }}
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