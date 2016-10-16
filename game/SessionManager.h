//
// Created by serg on 10/16/16.
//

#ifndef TIC_TAC_TOE_SESSIONMANAGER_H
#define TIC_TAC_TOE_SESSIONMANAGER_H

#include <functional>
#include <map>
#include "User.h"
#include <json.hpp>

template<typename Connection, typename Comparation = std::less<Connection>>
class SessionManager
{
public:
    using Callback = std::function<void(nlohmann::json &&)>
    using ConnectionsMap = std::map<Connection, std::shared_ptr<User>, Comparation>;

    void newConnection(Connection connection)
    {

    }

    void checkAuth(nlohmann::json &&request, Callback success, Callback failed)
    {

    }

    void closeConnection(Connection connection)
    {

    }

private:
    ConnectionsMap _connections;
};


#endif //TIC_TAC_TOE_SESSIONMANAGER_H
