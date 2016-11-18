//
// Created by serg on 11/17/16.
//

#ifndef TIC_TAC_TOE_SERVERTYPE_H
#define TIC_TAC_TOE_SERVERTYPE_H

#include "Server.h"
#include "MysqlStorage.h"
#include "WSServerImpl.h"

using ServerType = Server<WSServerImpl, MysqlStorage>;

#endif //TIC_TAC_TOE_SERVERTYPE_H
