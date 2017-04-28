#ifndef AUTHORIZETESTCASE_H
#define AUTHORIZETESTCASE_H

#include <QObject>
#include <TestCases/testcasebase.h>

class AuthorizeTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    AuthorizeTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};

#endif // AUTHORIZETESTCASE_H
