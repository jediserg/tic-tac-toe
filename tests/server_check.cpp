//
// Created by serg on 10/28/16.
//
#include <gtest/gtest.h>
#include "Server.h"
#include "FakeServer.h"
#include "SessionManager.h"
#include "DbFixture.h"
#include "ApiManager.h"

TEST_F(DbFixture, TestServer) {
    ApiManager api;
    Server<FakeServer, SessionManager<FakeServer::Connection>> server(api, 12345);
}
