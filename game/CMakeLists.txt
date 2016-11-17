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
        ConnectionPool.cpp ThreadPool.cpp ThreadPool.h log.cpp log.h PlayerBase.h Board.cpp Board.h Game.cpp Game.h Player.cpp Player.h GameManager.cpp GameManager.h)

add_library(game ${SOURCE_FILES})