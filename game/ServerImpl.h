//
// Created by serg on 10/15/16.
//

#ifndef TIC_TAC_TOE_SERVERIMPL_H
#define TIC_TAC_TOE_SERVERIMPL_H


#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>

typedef websocketpp::server<websocketpp::config::asio> server;

class ServerImpl
{
public:
    ServerImpl();

    void run();

private:
    server m_endpoint;
};


#endif //TIC_TAC_TOE_SERVERIMPL_H
