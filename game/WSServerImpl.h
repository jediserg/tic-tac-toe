//
// Created by serg on 10/15/16.
//

#ifndef TIC_TAC_TOE_SERVERIMPL_H
#define TIC_TAC_TOE_SERVERIMPL_H

#include <cstdint>
#include <string>
#include <websocket/server_ws.hpp>

class WSServerImpl
{
public:
    using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
    using Connection = WsServer::Connection;

    using NewConnectionHandler = std::function<void(Connection)>;
    using MessageHandler = std::function<void(Connection, std::string &&)>;
    using CloseHandler = std::function<void(Connection)>;

    WSServerImpl(std::string endpoint, uint16_t port, size_t thread_count);

    void run();

private:
    WsServer _server;
};

#endif //TIC_TAC_TOE_SERVERIMPL_H
