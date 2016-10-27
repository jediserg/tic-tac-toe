//
// Created by serg on 10/15/16.
//

#ifndef TIC_TAC_TOE_SERVER_H
#define TIC_TAC_TOE_SERVER_H

#include <functional>
#include <json.hpp>
#include "ApiManager.h"



template<class Impl, class SessionManager>
class Server
{
public:
    using Connection = Impl::Connection;
    using Compare    = Impl::Compare;
    using ServerType = Server<Impl, SessionManager>;

    Server(ApiManager &api_manager, uint16_t port) : _impl(port), _api_manager(api_manager)
    {}

    Server(const Server &) = delete;

    Server() = delete;

    Server &operator=(const Server &) = delete;

    void run()
    {
        _impl.setOnNewConnection(std::bind(&ServerType::onNewConnection, this, std::placeholders::_1));
        _impl.setOnClose(std::bind(&ServerType::onClose, this, std::placeholders::_1));
        _impl.setOnMessage(std::bind(&ServerType::onMessage, this, std::placeholders::_1, std::placeholders::_2));

        _impl.run();
    }

private:
    void onNewConnection(Impl::Connection connection)
    {
        _session_mgr.newConnection(connection);
    }

    void onMessage(Impl::Connection connection, nlohmann::json request)
    {
        auto api_name_it = request.find(ApiManager::API_FIELD);

        if (api_name_it == request.end()) {
            sendMessage(connection, {{"error", "No api specified"}});
            return;
        }

        if (!_api_manager.supportApi(*api_name_it)) {
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

        _session_mgr.processRequest(connection, std::move(request), std::bind(&Api::call, api, _1, _2,
                                                                              std::bind(&ServerType::sendMessage, this,
                                                                                        connection, _1)));
    }

    void onClose(Impl::Connection connection)
    {
        _session_mgr.closeConnection(connection);
    }

    void sendMessage(Impl::Connection connection, nlohmann::json message)
    {
        std::string json_message(message.dump());

        _impl.sendMessage(connection, std::move(json_message));
    }

    Impl _impl;
    SessionManager _session_mgr;
    ApiManager &_api_manager;
};


#endif //TIC_TAC_TOE_SERVER_H
