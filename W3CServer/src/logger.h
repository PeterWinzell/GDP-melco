#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QtMsgHandler>

class Logger : public QObject
{
    Q_OBJECT
public:
    Logger(QObject *parent = Q_NULLPTR);
    static int logLevel;
    static bool logToFile;
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString& message);
private:
    static void writeLogToFile(QString msg);
};

#endif // LOGGER_H
