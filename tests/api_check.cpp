//
// Created by serg on 10/14/16.
//

#include "gtest/gtest.h"
#include "Api.h"

using namespace nlohmann;

TEST(ApiCheck, TestBasicGetterSetters) {
    Api api("1.0");

    EXPECT_EQ("1.0", api.name());
    EXPECT_TRUE(api.isEnabled());

    api.disable();

    EXPECT_FALSE(api.isEnabled());

    api.enable();
    EXPECT_TRUE(api.isEnabled());
}

TEST(ApiCheck, TestHandlers) {
    Api api("1.0");

    bool testComandCompleted = false;

    const char* TEST_STRING = "This is a test string";

    api.setHandler("test", [TEST_STRING](json&& request, Api::Callback callback){
        EXPECT_EQ(request[Api::COMMAND_FIELD], "test");
        EXPECT_EQ(request["param"], "param");
        json result;
        result["test_string"] = TEST_STRING;
        callback(std::move(result));

        return true;
    });

    json request;
    request[Api::COMMAND_FIELD] = "test";
    request["param"]            = "param";

    EXPECT_TRUE(api.call(std::move(request), [&testComandCompleted, TEST_STRING](json&& response){
        testComandCompleted = true;

        EXPECT_EQ(TEST_STRING, response["test_string"]);
    }));

    EXPECT_TRUE(testComandCompleted);

    json no_command_request;
    no_command_request["param"] = "param";

    EXPECT_FALSE(api.call(std::move(no_command_request), [](json&& response){
        FAIL();
    }));

    json bad_command_request;
    bad_command_request[Api::COMMAND_FIELD] = "some_other_command";
    bad_command_request["param"]            = "param";

    EXPECT_FALSE(api.call(std::move(bad_command_request), [](json&& response){
        FAIL();
    }));
}

TEST(ApiCheck, TestDisabledHandler) {
    Api api("1.0");

    bool testComandCompleted = false;

    const char* TEST_STRING = "This is a test string";

    api.setHandler("test", [](json&& request, Api::Callback callback){
        //should never be called because api is disabled
        EXPECT_TRUE(false);

        return false;
    });

    api.disable();

    json request;
    request[Api::COMMAND_FIELD] = "test";
    request["param"]            = "param";

    EXPECT_FALSE(api.call(std::move(request), [&testComandCompleted, TEST_STRING](json&& response){
        testComandCompleted = true;

        FAIL();
    }));

    EXPECT_FALSE(testComandCompleted);
}