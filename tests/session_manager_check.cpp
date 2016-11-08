//
// Created by serg on 10/28/16.
//
#include <gtest/gtest.h>
#include <StoreInstance.h>
#include <SessionManager.h>
#include "DbFixture.h"

TEST_F(DbFixture, TestUserRegister) {
    auto& db = getMysqlStore(HOST, USER, PASSWORD, DB, 4);

    db.addTable<User>("user", "name", {
            {"name",    &User::getName},
            {"password",    &User::getPassword},
            {"win_count", [](const User *obj) { return std::to_string(obj->getWinCount()); }}
    });

    db.createTable<User>();

    ThreadPool::getInstance().waitForTasks();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    SessionManager<int> sm;

    sm.onRegisterRequest({{"name", "UserName"}, {"password", "test"}}, [](nlohmann::json&& response){
        auto error = response["error"];
        EXPECT_EQ(response[Api::COMMAND_FIELD], "userCreated");
    });

    ThreadPool::getInstance().waitForTasks();

    auto user = db.load<User>("UserName");

    EXPECT_TRUE((bool)user);

    EXPECT_EQ("UserName", user->getName());
    EXPECT_EQ("test", user->getPassword());
    EXPECT_EQ("0", std::to_string(user->getWinCount()));
}

TEST_F(DbFixture, TestUserLogin) {
    auto& db = getMysqlStore(HOST, USER, PASSWORD, DB, 4);

    db.addTable<User>("user", "name", {
            {"name",    &User::getName},
            {"password",    &User::getPassword},
            {"win_count", [](const User *obj) { return std::to_string(obj->getWinCount()); }}
    });

    db.createTable<User>();

    ThreadPool::getInstance().waitForTasks();

    db.save(User({{"name", "UserName"}, {"password", "test"}, {"win_count", "1"}}));

    ThreadPool::getInstance().waitForTasks();

    SessionManager<int> sm;
    sm.newConnection(1);
    sm.processRequest(1, {{Api::COMMAND_FIELD, "login"},
                          {"login",            "UserName"},
                          {"password",         "test"}}, [](nlohmann::json &&response) {
        EXPECT_TRUE(response.end() == response.find("error"));
        EXPECT_EQ(response[Api::COMMAND_FIELD], "loggedIn");
    }, [](std::shared_ptr<User> user, nlohmann::json &&request) {
        EXPECT_FALSE(true);
    });

    ThreadPool::getInstance().waitForTasks();
}