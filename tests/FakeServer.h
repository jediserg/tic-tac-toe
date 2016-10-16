//
// Created by serg on 10/16/16.
//

#ifndef TIC_TAC_TOE_FAKESERVER_H
#define TIC_TAC_TOE_FAKESERVER_H


#include <cstdint>
#include <functional>
#include <map>

class FakeServer
{
public:
    using Connection = int;

    struct ConnectionInfo
    {
        ConnectionInfo();

        ConnectionInfo(Connection c);

        Connection connection;
        bool is_open;
        std::string last_in_message;
        std::string last_out_message;
    };

    using ConnectionsMap = std::map<Connection, ConnectionInfo>;

    using NewConnectionHandler = std::function<void(Connection)>;
    using MessageHandler = std::function<void(Connection, std::string &&)>;
    using CloseHandler = std::function<void(Connection)>;

    FakeServer(uint16_t port);

    void run();

    void setOnNewConnection(NewConnectionHandler handler);

    void setOnMessage(MessageHandler handler);

    void setOnClose(CloseHandler handler);

    void sendMessage(Connection connection, std::string &&message);

    void newConnection(Connection connection);

    void gotMessage(Connection connection, std::string message);

    void closeConnection(Connection connection);

    uint16_t _port;
    bool _is_run;

    NewConnectionHandler _new_connection_handler;
    MessageHandler _message_handler;
    CloseHandler _close_handler;

    ConnectionsMap _connections;
};


#endif //TIC_TAC_TOE_FAKESERVER_H
