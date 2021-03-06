//
// Created by serg on 10/14/16.
//

#include <gtest/gtest.h>
#include <json.hpp>
#include "ApiManager.h"
#include "User.h"

using namespace nlohmann;

TEST(ApiManagerCheck, AddGetApi) {
    ApiManager api_manager;
    api_manager.setSupportedApi({"1.0", "2.0"});
    //create api
    Api &api1 = api_manager.api("1.0");
    Api &api2 = api_manager.api("2.0");

    EXPECT_EQ("1.0", api1.name());
    EXPECT_EQ("2.0", api2.name());

    EXPECT_TRUE(api1.isEnabled());
    EXPECT_TRUE(api2.isEnabled());

    api1.disable();
    api2.disable();

    //return existing api
    EXPECT_FALSE(api_manager.api("1.0").isEnabled());
    EXPECT_FALSE(api_manager.api("2.0").isEnabled());

    EXPECT_TRUE(api_manager.isApiSupported("1.0"));
    EXPECT_TRUE(api_manager.isApiSupported("2.0"));

    EXPECT_FALSE(api_manager.isApiSupported("3.0"));
}

TEST(ApiManagerCheck, CallApi) {
    ApiManager api_manager;
    api_manager.setSupportedApi({"1.0", "2.0"});
    //create api
    auto &api1 = api_manager.api("1.0");
    auto &api2 = api_manager.api("2.0");

    bool test_handler_was_called = false;

    std::shared_ptr<User> test_user = std::make_shared<User>(std::map<std::string, std::string>({{"name", "User"}}));

    api1.setHandler("test", [&test_handler_was_called](std::shared_ptr<User> user, json &&request) {
        json response{
                {"result", "OK"}
        };
        test_handler_was_called = true;
        return true;

    });

    json no_api_request{
            {"command", "test"},
            {"param",   "test"}
    };

    EXPECT_FALSE(api_manager.callApi(test_user, std::move(no_api_request)));

    json bad_api_request{
            {"api",     "3.0"},
            {"command", "test"},
            {"param",   "test"}
    };

    EXPECT_FALSE(api_manager.callApi(test_user, std::move(bad_api_request)));


    json good_request{
            {"api",              "1.0"},
            {Api::COMMAND_FIELD, "test"},
            {"param",            "param"}
    };
    EXPECT_TRUE(api_manager.callApi(test_user, std::move(good_request)));
    EXPECT_TRUE(test_handler_was_called);
}
