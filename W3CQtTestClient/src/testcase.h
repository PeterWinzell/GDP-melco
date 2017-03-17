#ifndef TESTCASE_H
#define TESTCASE_H

#include <QString>

enum class TestCase
{
    SUBSCRIBE_UNSUBSCRIBE,
    SUBSCRIBEALL_UNSUBSCRIBEALL,
    AUTHORIZE_SUCCESS,
    GET_VSS,
    SET_GET
};


struct TestCaseDescription
{
    int id;
    TestCase testCase;
    QString name;
    QString argument;
    QString description;
};


#endif // TESTCASE_H
