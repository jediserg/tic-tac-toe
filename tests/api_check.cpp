//
// Created by serg on 10/14/16.
//

#include <User.h>
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
    std::shared_ptr<User> test_user = std::make_shared<User>(std::map<std::string, std::string>({{"name", "Name"}}));

    bool testComandCompleted = false;

    const char *TEST_STRING = "This is a test string";

    api.setHandler("test",
                   [TEST_STRING, test_user, &testComandCompleted](std::shared_ptr<User> user, json &&request) {
                       EXPECT_EQ(request[Api::COMMAND_FIELD], "test");
                       EXPECT_EQ(request["param"], "param");
                       EXPECT_EQ(user, test_user);
                       EXPECT_EQ("Name", user->getName());
                       testComandCompleted = true;
                       json result;
                       result["test_string"] = TEST_STRING;

                       return true;
                   });

    json request;
    request[Api::COMMAND_FIELD] = "test";
    request["param"] = "param";

    EXPECT_TRUE(api.call(test_user, std::move(request)));

    EXPECT_TRUE(testComandCompleted);

    json no_command_request;
    no_command_request["param"] = "param";

    EXPECT_FALSE(api.call(test_user, std::move(no_command_request)));

    json bad_command_request;
    bad_command_request[Api::COMMAND_FIELD] = "some_other_command";
    bad_command_request["param"] = "param";

    EXPECT_FALSE(api.call(test_user, std::move(bad_command_request)));
}

TEST(ApiCheck, TestDisabledHandler) {
    Api api("1.0");
    std::shared_ptr<User> test_user = std::make_shared<User>(std::map<std::string, std::string>({{"name", "User"}}));

    bool testComandCompleted = false;

    const char *TEST_STRING = "This is a test string";

    api.setHandler("test", [](std::shared_ptr<User> user, json &&request) {
        //should never be called because api is disabled
        EXPECT_TRUE(false);

        return false;
    });

    api.disable();

    json request;
    request[Api::COMMAND_FIELD] = "test";
    request["param"] = "param";

    EXPECT_FALSE(api.call(test_user, std::move(request)));

    EXPECT_FALSE(testComandCompleted);
}