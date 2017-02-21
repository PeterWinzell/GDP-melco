#ifndef TST_W3CSERVER_H
#define TST_W3CSERVER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "w3cserver.h"

using namespace testing;


TEST(W3CServer, creation)
{
    W3CServer server(8080);
    ASSERT_EQ(typeid(server), typeid(W3CServer));
}

#endif // TST_W3CSERVER_H
