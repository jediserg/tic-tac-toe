/*!
 * \brief This class is owned by the Server and response for a simple user registration/login
 */

#ifndef TIC_TAC_TOE_SESSIONMANAGER_H
#define TIC_TAC_TOE_SESSIONMANAGER_H

#include "log.h"
#include <exception>
#include <functional>
#include <json.hpp>
#include <map>
#include "ApiManager.h"
#include "User.h"
#include "ThreadPool.h"
#include <mutex>
//#include "Store.h"
//#include "MysqlStorage.h"
#include <type_traits>

template<class Connection, class StoreClass>
class SessionManager {
public:
    using Callback = std::function<void(nlohmann::json &&)>;
    using ConnectionsMap = std::map<Connection, std::shared_ptr<User>>;
    using ConnectionByUserNameMap = std::map<std::string, Connection>;
    using ProcessRequest = std::function<void(std::shared_ptr<User>, nlohmann::json &&)>;
    //using StoreClass = Store<ConcreteStore>;

    static constexpr const char *REGISTER_COMMAND = "register";
    static constexpr const char *LOGIN_COMMAND = "login";

    static constexpr const char *USER_NAME_FIELD = "name";
    static constexpr const char *USER_LOGIN_FIELD = "login";
    static constexpr const char *ERROR_FIELD = "error";
    static constexpr const char *USER_PASSWORD_FIELD = "password";

    SessionManager(StoreClass &store) : _store(store) {

    }

    void onRegisterRequest(nlohmann::json &&json,
                           Api::Callback callback) const {
        auto name = json.find(USER_NAME_FIELD);
        auto password = json.find(USER_PASSWORD_FIELD);

        if (name == json.end() || password == json.end())
            return callback({{ERROR_FIELD, "No name or password"}});

        std::string str_name = *name;
        std::string str_password = *password;

        if (str_name.empty() || str_password.empty())
            return callback({{ERROR_FIELD, "Name or password empty"}});

        ThreadPool::getInstance().addTask([str_name, str_password, callback, this]() {
            try {
                _store.save(User({
                                                  {"name",      str_name},
                                                  {"password",  str_password},
                                                  {"win_count", "0"},
                                          }));
            } catch (const std::exception &e) {
                LOG_ERROR << "Couldn't create user:" << e.what();
                callback({{ERROR_FIELD, "Couldn't create User"}});
                return;
            } catch (...) {
                LOG_ERROR << "Couldn't create user";
                callback({{ERROR_FIELD, "Couldn't create User"}});
                return;
            }

            callback({{Api::COMMAND_FIELD, "userCreated"}});
        });
    }

    void processRequest(Connection connection, nlohmann::json &&request, Api::Callback response, ProcessRequest next) {
        std::unique_lock<std::mutex> guard(_mutex);

        auto it = _connections.find(connection);

        if (it == _connections.end())
            throw std::runtime_error("Connection not exists");

        guard.unlock();

        std::string command = request[Api::COMMAND_FIELD];

        if (command == LOGIN_COMMAND) {
            auto login = request.find(USER_LOGIN_FIELD);
            auto password = request.find(USER_PASSWORD_FIELD);

            if (login == request.end() || password == request.end())
                return response({{ERROR_FIELD, "Wrong login or password"}});

            std::string str_name = *login;
            std::string str_password = *password;

            if (str_name.empty() || str_password.empty())
                return response({{ERROR_FIELD, "Wrong login or password"}});

            ThreadPool::getInstance().addTask([str_name, str_password, response, next, connection, this]() {
                try {
                    auto user = _store.template load<User>(str_name);

                    if (user && user->getPassword() == str_password) {
                        std::lock_guard<std::mutex> guard(_mutex);

                        _connections[connection] = user;
                        _connections_by_user_name[str_name] = connection;
                    } else {
                        response({{ERROR_FIELD, "Bad login or password"}});
                    }
                } catch (const std::exception &e) {
                    LOG_ERROR << "Couldn't load user:" << e.what();
                    response({{ERROR_FIELD, "Couldn't load User"}});
                    return;
                } catch (...) {
                    LOG_ERROR << "Couldn't load user";
                    response({{ERROR_FIELD, "Couldn't load User"}});
                    return;
                }
            });

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
        std::lock_guard<std::mutex> guard(_mutex);

        auto it = _connections.find(connection);

        if (it != _connections.end())
            throw std::runtime_error("Connection is already exists");

        _connections[connection] = nullptr;
    }

    void closeConnection(Connection connection) {
        std::lock_guard<std::mutex> guard(_mutex);

        auto it = _connections.find(connection);
        if (it != _connections.end()) {
            if (it->second) {
                auto user_it = _connections_by_user_name.find(it->second->getName());
                _connections_by_user_name.erase(user_it);
            }
            _connections.erase(it);
        }


    }

    std::shared_ptr<User> getUser(Connection connection) {
        std::lock_guard<std::mutex> guard(_mutex);

        auto it = _connections.find(connection);

        if (it == _connections.end())
            throw std::runtime_error("Unknown connection");

        return it->second;
    }

    Connection getUserConnection(const std::string &user_name) {
        std::lock_guard<std::mutex> guard(_mutex);

        auto it = _connections_by_user_name.find(user_name);
        if (it == _connections_by_user_name.end())
            throw (std::invalid_argument(std::string("No connection for user:") + user_name));

        return it->second;
    }
private:
    StoreClass &_store;
    ConnectionsMap _connections;
    ConnectionByUserNameMap _connections_by_user_name;

    mutable std::mutex _mutex;
};


#endif //TIC_TAC_TOE_SESSIONMANAGER_H
