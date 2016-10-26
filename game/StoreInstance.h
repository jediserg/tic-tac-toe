//
// Created by serg on 10/26/16.
//

#ifndef TIC_TAC_TOE_STOREINSTANCE_H_H
#define TIC_TAC_TOE_STOREINSTANCE_H_H

#include "Store.h"
#include "MysqlStorage.h"

using MysqlStore = Store<MysqlStorage>;

MysqlStore& getMysqlStore(std::string host = "", std::string user = "", std::string password = "", std::string database = "", int pool_size = 0);

#endif //TIC_TAC_TOE_STOREINSTANCE_H_H
