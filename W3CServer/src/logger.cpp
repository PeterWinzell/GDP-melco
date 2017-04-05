#include "logger.h"

#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <iostream>

int Logger::logLevel = 0;
bool Logger::logToFile = true;
Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString& message)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    if (type == QtDebugMsg && logLevel >= 0)
    {
        QString msg = QString("[ Server ] [ %1 ] [ Debug    ] : %2 (%3:%4, %5)\n").arg(
                            QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),
                            message, context.file, QString::number(context.line),context.function);
        std::cerr << msg.toStdString().c_str();
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (type == QtInfoMsg && logLevel >= 1)
    {
        QString msg = QString("[ Server ] [ %1 ] [ Info     ] : %2 (%3:%4, %5)\n").arg(
                           QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),
                           message, context.file, QString::number(context.line), context.function);
        std::cerr << msg.toStdString().c_str();
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (type == QtWarningMsg && logLevel >= 2)
    {
        QString msg = QString("[ Server ] [ %1 ] [ Warning  ] : %2 (%3:%4, %5)\n").arg(
                           QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),
                           message, context.file, QString::number(context.line), context.function);
        std::cerr << msg.toStdString().c_str();
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (type == QtCriticalMsg && logLevel >= 3)
    {
        QString msg = QString("[ Server ] [ %1 ] [ Critical ] : %2 (%3:%4, %5)\n").arg(
                            QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),
                            message, context.file, QString::number(context.line), context.function);
        std::cerr << msg.toStdString().c_str();
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (type == QtFatalMsg && logLevel >= 4)
    {
        QString msg = QString("[ Server ] [ %1 ] [ Fatal    ] : %2 (%3:%4, %5)\n").arg(
                            QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),
                            message, context.file, QString::number(context.line), context.function);
        std::cerr << msg.toStdString().c_str();

        if(logToFile) { writeLogToFile(msg); }
        abort();
    }
}
void Logger::writeLogToFile(QString msg)
{
    static QFile file("logfile.log");

    if(!file.isOpen()) { file.open(QIODevice::Append); }
    QTextStream stream(&file);

    stream << msg;// << endl;
    stream.flush();
}
