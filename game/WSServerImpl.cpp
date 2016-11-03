//
// Created by serg on 10/15/16.
//

#include <cstdint>
#include <cstddef>
#include <string>
#include <memory>
#include "WSServerImpl.h"

WSServerImpl::WSServerImpl(std::string endpoint, uint16_t port, size_t thread_count) : _server(port, thread_count) {
    auto &handlers = _server.endpoint["^/tic-tac-toe/?$"];

    handlers.onopen = [this](WSServerImpl::Connection connection) {
        _new_connection_handler(connection);
    };

    handlers.onmessage = [this](WSServerImpl::Connection connection, std::shared_ptr<WsServer::Message> message) {
        _message_handler(connection, message->string());
    };

    handlers.onclose = [this](WSServerImpl::Connection connection, int status, const std::string & /*reason*/) {
        _close_handler(connection);
    };
}

void WSServerImpl::run() {
    _server.start();
}

void WSServerImpl::setOnNewConnection(WSServerImpl::NewConnectionHandler handler) {
    _new_connection_handler = handler;
}

void WSServerImpl::setOnMessage(WSServerImpl::MessageHandler handler) {
    _message_handler = handler;
}

void WSServerImpl::setOnClose(WSServerImpl::CloseHandler handler) {
    _close_handler = handler;
}

void WSServerImpl::sendMessage(WSServerImpl::Connection connection, std::string &&message) {
    auto send_stream = std::make_shared<WsServer::SendStream>();
    *send_stream << std::move(message);

    _server.send(connection, send_stream, [](const boost::system::error_code &ec) {
        if (ec) {
            std::cout << "Server: Error sending message. " <<
                      "Error: " << ec << ", error message: " << ec.message() << std::endl;
        }
    });
}

void WSServerImpl::closeConnection(WSServerImpl::Connection connection) {
    static const int CLOSE_NORMAL = 1000;

    _server.send_close(connection, CLOSE_NORMAL);
}
