#ifndef SETMANYTESTCASE_H
#define SETMANYTESTCASE_H

#include <QObject>
#include <TestCases/testcasebase.h>

class SetManyTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    SetManyTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};

#endif // SETMANYTESTCASE_H
