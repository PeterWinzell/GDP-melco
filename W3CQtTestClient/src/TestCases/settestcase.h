#ifndef SETTESTCASE_H
#define SETTESTCASE_H

#include <QObject>
#include <TestCases/testcasebase.h>

class SetTestCase : public TestCaseBase
{
    Q_OBJECT
public:
    SetTestCase(QString testClientId, int& requestid, QObject *parent = NULL);
    void startTest(QWebSocket* webSocket);

signals:
    void finished(bool result);

public slots:
    void forceStop();
    void onTextMessageReceived(QString message);
};

#endif // SETTESTCASE_H
