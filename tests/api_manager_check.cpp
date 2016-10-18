//
// Created by serg on 10/14/16.
//

#include <gtest/gtest.h>
#include <json.hpp>
#include <memory>
#include "ApiManager.h"
#include "User.h"

using namespace nlohmann;

TEST(ApiManagerCheck, AddGetApi) {
    ApiManager api_manager;

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

    EXPECT_TRUE(api_manager.supportApi("1.0"));
    EXPECT_TRUE(api_manager.supportApi("2.0"));

    EXPECT_FALSE(api_manager.supportApi("3.0"));


}

TEST(ApiManagerCheck, CallApi) {
    ApiManager api_manager;

    //create api
    auto api1 = api_manager.api("1.0");
    auto api2 = api_manager.api("2.0");

    std::shared_ptr<User> test_user = std::make_shared<User>("Name");

    api1.setHandler("test", [](std::shared_ptr<User> user, json &&request, Api::Callback callback) {
        json response {
               {"result", "OK"}
        };

        callback(std::move(response));

        return true;

    });

    json no_api_request {
            {"command", "test"},
            {"param", "test"}
    };

    EXPECT_FALSE(api_manager.callApi(test_user, std::move(no_api_request), [](json &&response) {
        FAIL();
    }));

    json bad_api_request {
            {"api", "3.0"},
            {"command", "test"},
            {"param", "test"}
    };

    EXPECT_FALSE(api_manager.callApi(test_user, std::move(bad_api_request), [](json &&response) {
        FAIL();
    }));


    json good_request {
            {"api", "1.0"},
            {Api::COMMAND_FIELD, "test"},
            {"param", "param"}
    };
    EXPECT_TRUE(api_manager.callApi(test_user, std::move(good_request), [](json &&response) {
        ;
    }));
}
