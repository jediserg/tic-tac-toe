//
// Created by serg on 10/28/16.
//
//#define BOOST_LOG_DYN_LINK 1
#include <gtest/gtest.h>
#include "Server.h"
#include "FakeServer.h"
#include "DbFixture.h"
#include "MysqlStorage.h"

TEST_F(DbFixture, TestServer) {
    Store<MysqlStorage> db(HOST, USER, PASSWORD, DB, 4);

    db.addTable<User>("user", "name", {
            {"name",      &User::getName},
            {"password",  &User::getPassword},
            {"win_count", [](const User *obj) { return std::to_string(obj->getWinCount()); }}
    });

    db.createTable<User>();

    ThreadPool::getInstance().waitForTasks();

    ApiManager api_manager;

    api_manager.setSupportedApi({"1.0", "1.2", "1.3"});

    bool command1_compleat = false;
    api_manager.setHandler("1.0", "command1",
                           [&](std::shared_ptr<User> user, nlohmann::json &&request, Api::Callback c) {
                               EXPECT_TRUE(user != nullptr);
                               command1_compleat = true;

                               return true;
                           });

    FakeServer fakeServer;
    Server<FakeServer, MysqlStorage> server(api_manager, fakeServer, db);
    server.run();

    EXPECT_TRUE(fakeServer._is_run);

    fakeServer.newConnection(1);

    const auto &connection = fakeServer._connections.find(1);

    EXPECT_EQ(1, connection->first);
    EXPECT_TRUE(connection->second.is_open);

    EXPECT_TRUE(connection->second.last_in_message.empty());

    std::string register_msg = R"STR(
{"api":"1.0",
"command":"register",
"name":"UserLogin",
"password":"123456"
})STR";

    fakeServer.gotMessage(1, register_msg);

    ThreadPool::getInstance().waitForTasks();

    nlohmann::json response_register = nlohmann::json::parse(connection->second.last_out_message);

    EXPECT_TRUE(connection->second.is_open);

    EXPECT_EQ(connection->second.last_in_message, register_msg);
    EXPECT_EQ(response_register["command"], "userCreated");


    fakeServer.gotMessage(1, R"STR(
{"api":"1.0",
"command":"login",
"login":"UserLogin",
"password":"123456"
})STR");
    ThreadPool::getInstance().waitForTasks();
    nlohmann::json response_login = nlohmann::json::parse(connection->second.last_out_message);

    EXPECT_EQ(response_login["command"], "loggedIn");
}
