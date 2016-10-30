//
// Created by serg on 10/16/16.
//

#ifndef TIC_TAC_TOE_SESSIONMANAGER_H
#define TIC_TAC_TOE_SESSIONMANAGER_H

#include <exception>
#include <functional>
#include <json.hpp>
#include <map>
#include "ApiManager.h"
#include "User.h"
#include "StoreInstance.h"
#include <StoreInstance.h>

template<typename Connection, typename Comparation = std::less<Connection>>
class SessionManager {
public:
    using Callback = std::function<void(nlohmann::json &&)>;
    using ConnectionsMap = std::map<Connection, std::shared_ptr<User>, Comparation>;
    using ProcessRequest = std::function<void(std::shared_ptr<User>, nlohmann::json &&)>;

    static constexpr const char *REGISTER_COMMAND = "register";
    static constexpr const char *LOGIN_COMMAND = "login";

    static constexpr const char *USER_NAME_FIELD = "name";
    static constexpr const char *ERROR_FIELD = "error";
    static constexpr const char *USER_PASSWORD_FIELD = "password";

    SessionManager() {
    }

    void onRegisterRequest(nlohmann::json &&json,
                           Api::Callback callback) {
        auto name = json.find(USER_NAME_FIELD);
        auto password = json.find(USER_PASSWORD_FIELD);

        if (name == json.end() || password == json.end())
            return callback({{ERROR_FIELD, "Wrong login or password"}});

        std::string str_name = *name;
        std::string str_password = *password;

        if (str_name.empty() || str_password.empty())
            return callback({{ERROR_FIELD, "Wrong login or password"}});

        try {
            getMysqlStore().save(User({
                                              {"name",      str_name},
                                              {"password",  str_password},
                                              {"win_count", "0"},
                                      }));
        } catch (const std::exception &e) {
            std::cout << "Couldn't create user:" << e.what();
            callback({{ERROR_FIELD, "Couldn't create User"}});
            return;
        } catch (...) {
            std::cout << "Couldn't create user";
            callback({{ERROR_FIELD, "Couldn't create User"}});
            return;
        }

        callback({{Api::COMMAND_FIELD, "userCreated"}});
    }

    void processRequest(Connection connection, nlohmann::json &&request, Api::Callback response, ProcessRequest next) {
        auto it = _connections.find(connection);

        if (it == _connections.end())
            throw std::runtime_error("Connection not exists");

        std::string command = request[Api::COMMAND_FIELD];

        if (command == LOGIN_COMMAND) {
            auto name = request.find(USER_NAME_FIELD);
            auto password = request.find(USER_NAME_FIELD);

            if (name == request.end() || password == request.end())
                return response({{ERROR_FIELD, "Wrong login or password"}});

            std::string str_name = *name;
            std::string str_password = *password;

            if (str_name.empty() || str_password.empty())
                return response({{ERROR_FIELD, "Wrong login or password"}});

            try {
                auto user = getMysqlStore().load<User>("name");

                if (user->getPassword() == str_password)
                    _connections[connection] = user;
            } catch (const std::exception &e) {
                std::cout << "Couldn't create user:" << e.what();
                response({{ERROR_FIELD, "Couldn't create User"}});
                return;
            } catch (...) {
                std::cout << "Couldn't create user";
                response({{ERROR_FIELD, "Couldn't create User"}});
                return;
            }

            response({{Api::COMMAND_FIELD, "loggedIn"}});
        } else {
            if (command == REGISTER_COMMAND) {
                onRegisterRequest(std::move(request), response);
                return;
            }

            if (it->second)
                next(nullptr, std::move(request));
            else
                next(it->second, std::move(request));
        }
    }

    void newConnection(Connection connection) {
        auto it = _connections.find(connection);

        if (it != _connections.end())
            throw std::runtime_error("Connection is already exists");

        _connections[connection] = nullptr;
    }

    void closeConnection(Connection connection) {
        _connections.erase(connection);
    }

    std::shared_ptr<User> getUser(Connection connection) {
        auto it = _connections.find(connection);

        if (it == _connections.end())
            throw std::runtime_error("Unknown connection");

        return it->second;
    }
private:
    ConnectionsMap _connections;
};


#endif //TIC_TAC_TOE_SESSIONMANAGER_H
