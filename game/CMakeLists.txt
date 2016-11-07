project(game)

set(SOURCE_FILES
        Api.h
        Api.cpp
        ApiManager.h
        ApiManager.cpp
        Server.h
        WSServerImpl.cpp
        WSServerImpl.h
        User.cpp
        User.h
        Store.h
        Store.cpp
        Validatable.h
        MysqlStorage.cpp
        ConnectionPool.cpp StoreInstance.h StoreInstance.cpp ThreadPool.cpp ThreadPool.h log.cpp log.h PlayerBase.h Board.cpp Board.h)

add_library(game ${SOURCE_FILES})