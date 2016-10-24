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
        Store.h
        Store.cpp
        Validatable.h DbStore.cpp)

add_library(game ${SOURCE_FILES})