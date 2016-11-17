/*!
 * \brief General class that process incomming messages usin Impl strategy to work with different protocols
 */

#ifndef TIC_TAC_TOE_SERVER_H
#define TIC_TAC_TOE_SERVER_H

#include <functional>
#include <json.hpp>
#include "ApiManager.h"
#include "log.h"
#include "SessionManager.h"
#include "Store.h"

template<class Impl, class ConcreteStore>
class Server {
public:
    using Connection = typename Impl::Connection;

    typedef Server<Impl, ConcreteStore> ServerType;

    Server(ApiManager &api_manager, Impl &impl, Store<ConcreteStore> &_store) : _impl(impl),
                                                                                _session_mgr(_store),
                                                                                _api_manager(api_manager) {}

    Server(const Server &) = delete;

    Server() = delete;

    Server &operator=(const Server &) = delete;

    void run() {
        LOG_DEBUG << "Start server";
        _impl.setOnNewConnection(std::bind(&ServerType::onNewConnection, this, std::placeholders::_1));
        _impl.setOnClose(std::bind(&ServerType::onClose, this, std::placeholders::_1));
        _impl.setOnMessage(std::bind(&ServerType::onMessage, this, std::placeholders::_1, std::placeholders::_2));

        _impl.run();
    }

private:
    void onNewConnection(Connection connection) {
        LOG_DEBUG << "New connection";
        _session_mgr.newConnection(connection);
    }

    void onMessage(Connection connection, std::string &&message) {
        auto request = nlohmann::json::parse(message);

        auto api_name_it = request.find(ApiManager::API_FIELD);

        if (api_name_it == request.end()) {
            sendMessage(connection, {{"error", "No api specified"}});
            return;
        }

        if (!_api_manager.isApiSupported(*api_name_it)) {
            sendMessage(connection, {{"error", "Unsupported Api"}});
            return;
        }

        auto command_it = request.find(Api::COMMAND_FIELD);

        if (command_it == request.end()) {
            sendMessage(connection, {{"error", "Command is not specified"}});
            return;
        }

        const auto &api = _api_manager.api(*api_name_it);

        using namespace std::placeholders;

        _session_mgr.processRequest(connection, std::move(request),
                                    std::bind(&ServerType::sendMessage, this, connection, _1),
                                    [connection, &api, this](std::shared_ptr<User> user, nlohmann::json &&request) {
                                        api.call(user, std::move(request),
                                                 std::bind(&ServerType::sendMessage, this, connection, _1));
                                    });
    }

    void onClose(Connection connection) {
        _session_mgr.closeConnection(connection);
    }

    void sendMessage(Connection connection, nlohmann::json &&message) {
        try {
            std::string json_message = message.dump();
            _impl.sendMessage(connection, std::move(json_message));
        } catch (const std::invalid_argument &e) {
            LOG_ERROR << e.what();
        }
        catch (const std::exception &e) {
            LOG_ERROR << e.what();
        } catch (...) {
            LOG_ERROR << "Couldn't parse json";
        }
    }

    void sendMessageToUser(const std::string &user_name, nlohmann::json &&message) {
        try {
            auto connection = _session_mgr.getUserConnection(user_name);
            sendMessage(connection, std::move(message));
        } catch (const std::invalid_argument &e) {
            LOG_ERROR << e.what();
        }
        catch (const std::exception &e) {
            LOG_ERROR << e.what();
        } catch (...) {
            LOG_ERROR << "Couldn't parse json";
        }
    }

    Impl &_impl;
    SessionManager<Connection, Store<ConcreteStore>> _session_mgr;
    ApiManager &_api_manager;
};


#endif //TIC_TAC_TOE_SERVER_H
