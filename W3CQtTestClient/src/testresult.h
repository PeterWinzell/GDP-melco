#ifndef TESTRESULT_H
#define TESTRESULT_H

#include <QDateTime>
#include "TestCases/testcase.h"
#include <QPair>

struct TestResult
{
    QList<QPair<QString, QString>> output;

    bool outcome;
    QDateTime started;
    QDateTime ended;
    TestCase testcase;
    QString description;
};

#endif // TESTRESULT_H
