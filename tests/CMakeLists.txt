project(tests)

add_subdirectory(lib/gtest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${game_SOURCE_DIR})

add_executable(game_tests
        api_check.cpp
        api_manager_check.cpp FakeServer.cpp FakeServer.h ../game/SessionManager.h db_check.cpp FakeStore.cpp FakeStore.h session_manager_check.cpp DbFixture.h server_check.cpp thread_pool_check.cpp check_board.cpp)

target_link_libraries(game_tests boost_log gtest gtest_main game mysqlcppconn)