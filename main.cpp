#include <iostream>
#include "game/log.h"
#include "game/Api.h"
#include "game/ApiManager.h"
#include "game/ThreadPool.h"
#include "game/User.h"
#include <cxxopts/cxxopts.hpp>
#include "game/ServerType.h"



int main(int argc, char **argv) {
    bool install = false;
    std::string host;
    std::string user;
    std::string password;
    std::string database;

    cxxopts::Options options(argv[0], "Backend for tic-tac-toe game");
    options.add_options()
            ("i,install", "Create tables", cxxopts::value<bool>(install))
            ("h,host", "Database host", cxxopts::value<std::string>(host))
            ("u,user", "Database user", cxxopts::value<std::string>(user))
            ("p,password", "Database user's password", cxxopts::value<std::string>(password))
            ("d,database", "Database name", cxxopts::value<std::string>(database))
            ("help", "Help message");

    options.parse(argc, argv);

    if (options.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (user.size() == 0 || password.size() == 0 || database.size() == 0 || host.size() == 0) {
        std::cout << "You should specify user name, user password and database" << std::endl;
        exit(1);
    }

    ThreadPool::getInstance(2);

    Store<MysqlStorage> db{host, user, password, database, 4};

    db.addTable<User>("user", "name", {
            {"name",      &User::getName},
            {"password",  &User::getPassword},
            {"win_count", [](const User *obj) { return std::to_string(obj->getWinCount()); }}
    });

    if (install) {
        db.createTable<User>();
        ThreadPool::getInstance().waitForTasks();
    }
    ApiManager api_manager;
    api_manager.setSupportedApi({"1.0", "2.0"});

    WSServerImpl impl("localhost", 1234, 2);

    ServerType server(api_manager, impl, db);
    server.run();

    return 0;
}