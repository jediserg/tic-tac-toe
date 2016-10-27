//
// Created by serg on 10/16/16.
//

#include "FakeServer.h"

FakeServer::FakeServer(uint16_t port)
        : _port(port), _is_run(false) {}

void FakeServer::run() {
    _is_run = true;
}

void FakeServer::setOnNewConnection(FakeServer::NewConnectionHandler handler) {
    _new_connection_handler = handler;
}

void FakeServer::setOnMessage(FakeServer::MessageHandler handler) {
    _message_handler = handler;
}

void FakeServer::setOnClose(FakeServer::CloseHandler handler) {
    _close_handler = handler;
}

void FakeServer::newConnection(FakeServer::Connection connection) {
    _connections[connection] = ConnectionInfo(connection);
    _new_connection_handler(connection);
}

void FakeServer::gotMessage(FakeServer::Connection connection, std::string message) {
    _message_handler(connection, std::move(message));
}

void FakeServer::closeConnection(FakeServer::Connection connection) {
    _close_handler(connection);
}

void FakeServer::sendMessage(FakeServer::Connection connection, std::string &&message) {
    auto &info = _connections[connection];

    if (info.is_open) {
        info.last_out_message = std::move(message);
    }
}

FakeServer::ConnectionInfo::ConnectionInfo(FakeServer::Connection c)
        : connection(c), is_open(true) {
}

FakeServer::ConnectionInfo::ConnectionInfo()
        : connection(-1), is_open(false) {

}
