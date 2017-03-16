#ifndef TESTRESULT_H
#define TESTRESULT_H

#include <QDateTime>
#include "testcase.h"

struct TestResult
{
    bool outcome;
    QDateTime started;
    QDateTime ended;
    TestCase testcase;
    QString description;
};

#endif // TESTRESULT_H
