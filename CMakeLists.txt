cmake_minimum_required(VERSION 3.6)
project(tic_tac_toe)

add_definitions(-DBOOST_LOG_DYN_LINK=1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)


include_directories(3rdparty 3rdparty/json ../../dev/mysql/include 3rdparty/json)
link_directories(../../dev/mysql/lib)

add_subdirectory(game)
add_subdirectory(tests)

add_executable(tic_tac_toe ${SOURCE_FILES})
target_link_libraries(tic_tac_toe game boost_log mysqlcppconn crypto)

