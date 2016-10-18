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

template<typename Connection, typename Comparation = std::less<Connection>>
class SessionManager
{
public:
    using Callback = std::function<void(nlohmann::json &&)>
    using ConnectionsMap = std::map<Connection, std::shared_ptr<User>, Comparation>;

    static constexpr const char *REGISTER_COMMAND = "register";
    static constexpr const char *LOGIN_COMMAND = "register";

    static constexpr const char *USER_NAME_FIELD = "name";
    static constexpr const char *USER_PASSWORD_FIELD = "name";

    SessionManager() {
        ApiManager::get().setHandlerForAllApi(REGISTER_COMMAND,
                                              [this](std::shared_ptr<User> user, nlohmann::json &&json,
                                                     Api::Callback callback) {

                                              });

        ApiManager::get().setHandlerForAllApi(LOGIN_COMMAND, [this](std::shared_ptr<User> user, nlohmann::json &&json,
                                                                    Api::Callback callback) {

        });
    }

    void newConnection(Connection connection)
    {
        auto it = _connections.find(connection);

        if (it != _connections.end())
            throw std::runtime_error("Connection is already exists");

        _connections[connection] = nullptr;
    }

    void onMessage(Connection connection, nlohmann::json &&request, Callback success, Callback failed)
    {
        auto it = _connections.find(connection);
        if (it == _connections.end())
            throw std::runtime_error("Unknown connection");

        auto command = request[REGISTER_COMMAND];
        if (!it->second) {
            if (command == REGISTER_COMMAND) {
                failed({{"error", "You must register before you can send commands"}});
                return;
            }

            auto user_name =

                    _connections[connection] = std::make_shared();
        }
    }

    void closeConnection(Connection connection)
    {
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
