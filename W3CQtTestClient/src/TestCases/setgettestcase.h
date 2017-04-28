#ifndef SETGETTESTCASE_H
#define SETGETTESTCASE_H

#include <QObject>
#include <TestCases/testcasebase.h>

class SetGetTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    SetGetTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};

#endif // SETGETTESTCASE_H
