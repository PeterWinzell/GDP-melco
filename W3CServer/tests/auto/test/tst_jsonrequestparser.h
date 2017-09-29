#ifndef TST_JSONREQUESTPARSER_H
#define TST_JSONREQUESTPARSER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "jsonrequestparser.h"

using namespace testing;


void ErrorHelper(QString jsonString, QString errorMsg)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    EXPECT_EQ(request->getAction(), ERROR) << errorMsg.toStdString();

}

void GetRequestHelper(QString jsonString)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    ASSERT_EQ(request->getAction(), GET);
    EXPECT_EQ(request->getRequestId(), "8756");
    EXPECT_EQ(request->getSignalPath(), "Signal.Drivetrain.InternalCombustionEngine.RPM");

}
void SetRequestHelper(QString jsonString)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    ASSERT_EQ(request->getAction(), SET);
    EXPECT_EQ(request->getRequestId(), "8912");
    EXPECT_EQ(request->getSignalPath(), "Signal.Drivetrain.InternalCombustionEngine.RPM");
    EXPECT_EQ(request->getValue(), 2000);

}
void SubscribeRequestHelper(QString jsonString)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    ASSERT_EQ(request->getAction(), SUBSCRIBE);
    EXPECT_EQ(request->getRequestId(), "5264");
    EXPECT_EQ(request->getSignalPath(), "Signal.Drivetrain.Transmission.TripMeter");

}
void UnsubscribeRequestHelper(QString jsonString)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    ASSERT_EQ(request->getAction(), UNSUBSCRIBE);
    EXPECT_EQ(request->getRequestId(), "5264");
    EXPECT_EQ(request->getSubscriptionId(), "102");

}
void UnsubscribeAllRequestHelper(QString jsonString)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    ASSERT_EQ(request->getAction(), UNSUBSCRIBEALL);
    EXPECT_EQ(request->getRequestId(), "5264");

}
void AuthorizeRequestHelper(QString jsonString)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    ASSERT_EQ(request->getAction(), AUTHORIZE);
    //EXPECT_EQ(request->getTokens(), "");  // TODO
    EXPECT_EQ(request->getRequestId(), "1");

}
void GetVSSRequestHelper(QString jsonString)
{
    JSONRequestParser jsonParser;
    QSharedPointer<VISSRequest> request = jsonParser.parseJson(jsonString);

    ASSERT_EQ(request->getAction(), GETMETADATA);
    EXPECT_EQ(request->getRequestId(), "3874");
    EXPECT_EQ(request->getSignalPath(), "Signal.Drivetrain.InternalCombustionEngine.RPM");

}


TEST(JSONRequestParser, getRequest_invalid_json)
{
    ErrorHelper("{\"action\": \"get\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"", "invalid json didn\'t return Error.");
    ErrorHelper("","empty input didn\'t return Error.");
    ErrorHelper("Hello world!","no json input didn\'t return Error.");
}
TEST(JSONRequestParser, getRequest_correct_json_but_not_ours)
{
    ErrorHelper("{\"action\": \"get\"\"}", "valid json not according to our specs didn\'t return Error.");

}

TEST(JSONRequestParser, getRequest_invalid_action)
{
    ErrorHelper("{\"action\": \"GET\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",   "invalid action didn\'t return Error.");

}
TEST(JSONRequestParser, getRequest_nonexistant_action)
{
    ErrorHelper("{\"action\": \"put\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",     "invalid action didn\'t return Error.");
    ErrorHelper("{\"action\": \"\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",        "invalid action didn\'t return Error.");

}
TEST(JSONRequestParser, getRequest_no_action_field)
{
    ErrorHelper("{\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",                         "no action didn\'t return Error.");
}

TEST(JSONRequestParser, getRequest_invalid_requestid)
{
    ErrorHelper("{\"action\": \"get\", \"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"\"}",      "invalid requestId didn\'t return Error.");
    ErrorHelper("{\"action\": \"get\", \"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"abc\"}",   "invalid requestId didn\'t return Error.");
}
TEST(JSONRequestParser, getRequest_no_requestid_field)
{
    ErrorHelper("{\"action\": \"get\", \"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\"}",                            "no requestId didn\'t return Error.");
}

TEST(JSONRequestParser, getRequest_invalid_path)
{
    ErrorHelper("{\"action\": \"get\",\"path\": \"\",\"requestId\": \"8756\"}",                                                   "invalid path (#1) didn\'t return Error.");
    ErrorHelper("{\"action\": \"get\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.\",\"requestId\": \"8756\"}",        "invalid path (#2) didn\'t return Error.");
    ErrorHelper("{\"action\": \"get\",\"path\": \".Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",           "invalid path (#3) didn\'t return Error.");
    ErrorHelper("{\"action\": \"get\",\"path\": \"Signal..InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",               "invalid path (#4) didn\'t return Error.");
    ErrorHelper("{\"action\": \"get\",\"path\": \"Signal.123.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",            "invalid path (#5) didn\'t return Error.");
    ErrorHelper("{\"action\": \"get\",\"path\": \"Signal.**.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}",             "invalid path (#6) didn\'t return Error.");
}

TEST(JSONRequestParser, getRequest_no_path_field)
{
    ErrorHelper("{\"action\": \"get\",\"requestId\": \"8756\"}", "no path didn\'t return Error.");

}

TEST(JSONRequestParser, getRequest_path_with_asterisk)
{
    ErrorHelper("{\"action\": \"get\",\"requestId\": \"8756\"}", "no path didn\'t return Error.");

}

TEST(JSONRequestParser, getRequest_viss_get_request)
{
    GetRequestHelper("{\"action\": \"get\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8756\"}");

}

TEST(JSONRequestParser, getRequest_viss_set_request)
{
    SetRequestHelper("{\"action\": \"set\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"value\": 2000,\"requestId\": \"8912\"}");
}
TEST(JSONRequestParser, getRequest_viss_set_request_no_value)
{
    ErrorHelper("{\"action\": \"set\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"requestId\": \"8912\"}",     "no value didn\'t return Error.");
}
TEST(JSONRequestParser, getRequest_viss_set_request_invalid_value)
{
    ErrorHelper("{\"action\": \"set\",\"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\",\"value\": abc,\"requestId\": \"8912\"}",
                "invalid value didn\'t return Error.");
}

TEST(JSONRequestParser, getRequest_viss_subscribe_request)
{
    SubscribeRequestHelper("{\"action\": \"subscribe\",\"path\": \"Signal.Drivetrain.Transmission.TripMeter\",\"requestId\": \"5264\"}");
}

TEST(JSONRequestParser, getRequest_viss_unsubscribe_request)
{
    UnsubscribeRequestHelper("{\"action\": \"unsubscribe\",\"subscriptionId\": \"102\",\"requestId\": \"5264\"}");
}

TEST(JSONRequestParser, getRequest_viss_unsubscribeall_request)
{
    UnsubscribeAllRequestHelper("{\"action\": \"unsubscribeAll\",\"requestId\": \"5264\"}");
}

TEST(JSONRequestParser, getRequest_viss_authorize_request)
{
    AuthorizeRequestHelper("{ \"action\": \"authorize\", \"tokens\":{ \"authorization\": \"a-token-value\" }, \"requestId\": \"1\" }");
}

TEST(JSONRequestParser, getRequest_viss_getvss_request)
{
    GetVSSRequestHelper("{ \"action\": \"getVSS\", \"path\": \"Signal.Drivetrain.InternalCombustionEngine.RPM\", \"requestId\": \"3874\"}");
}


#endif // TST_JSONREQUESTPARSER_H
