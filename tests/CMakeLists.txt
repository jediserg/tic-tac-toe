project(tests)

add_subdirectory(lib/gtest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${game_SOURCE_DIR})

add_executable(game_tests
        api_check.cpp
        api_manager_check.cpp)

target_link_libraries(game_tests gtest gtest_main game)