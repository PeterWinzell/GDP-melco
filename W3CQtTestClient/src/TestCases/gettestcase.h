#ifndef GETTESTCASE_H
#define GETTESTCASE_H

#include <QObject>
#include <TestCases/testcase.h>

class GetTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    GetTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};

#endif // GETTESTCASE_H
