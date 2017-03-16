<<<<<<< HEAD
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
=======
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
>>>>>>> 8426b50bc0330aee1954172cdf6a7e967943eb39
