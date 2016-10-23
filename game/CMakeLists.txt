project(game)

set(SOURCE_FILES
        Api.h
        Api.cpp
        ApiManager.h
        ApiManager.cpp
        Server.h
        ServerImpl.cpp
        ServerImpl.h
        User.cpp
        User.h
        Db.h
        Db.cpp
        Validatable.h)

add_library(game ${SOURCE_FILES})