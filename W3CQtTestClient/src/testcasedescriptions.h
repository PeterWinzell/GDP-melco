#ifndef TESTCASEDESCRIPTIONS_H
#define TESTCASEDESCRIPTIONS_H

#include <QObject>
#include "testcase.h"


class TestCaseDescriptions
{
public:
    struct TestCaseDescription
    {
        TestCaseDescription() {}
        TestCaseDescription(int id, TestCase testCase, QString name, QString argument, QString description) :
            m_id(id), m_testCase(testCase), m_name(name), m_argument(argument), m_description(description) {}

        int m_id;
        TestCase m_testCase;
        QString m_name;
        QString m_argument;
        QString m_description;
    };
    TestCaseDescriptions();
    TestCaseDescription *getDescription(int testCaseId) { return &m_descriptions[testCaseId]; }
    TestCaseDescription *getDescription(TestCase testCase) { return getDescription((int)testCase); }
    TestCaseDescription *getDescriptions() { return m_descriptions; }

private:
    TestCaseDescription m_descriptions[(int)TestCase::NR_OF_TESTCASES];
};

#endif // TESTCASEDESCRIPTIONS_H
