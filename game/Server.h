/*!
 * \brief General class that process incomming messages usin Impl strategy to work with different protocols
 */

#ifndef TIC_TAC_TOE_SERVER_H
#define TIC_TAC_TOE_SERVER_H

#include <functional>
#include <json.hpp>
#include "ApiManager.h"


template<class Impl, class SM>
class Server {
public:
    using Connection = typename Impl::Connection;
    using Compare    = typename Impl::Compare;
    typedef Server<Impl, SM> ServerType;

    Server(ApiManager &api_manager, Impl &impl) : _impl(impl), _api_manager(api_manager) {}

    Server(const Server &) = delete;

    Server() = delete;

    Server &operator=(const Server &) = delete;

    void run() {
        _impl.setOnNewConnection(std::bind(&ServerType::onNewConnection, this, std::placeholders::_1));
        _impl.setOnClose(std::bind(&ServerType::onClose, this, std::placeholders::_1));
        _impl.setOnMessage(std::bind(&ServerType::onMessage, this, std::placeholders::_1, std::placeholders::_2));

        _impl.run();
    }

private:
    void onNewConnection(Connection connection) {
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

        auto &api = _api_manager.api(*api_name_it);

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
            std::cout << e.what() << std::endl;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        } catch (...) {
            std::cout << "Couldn't parse json" << std::endl;
        }
    }

    Impl &_impl;
    SM _session_mgr;
    ApiManager &_api_manager;
};


#endif //TIC_TAC_TOE_SERVER_H
