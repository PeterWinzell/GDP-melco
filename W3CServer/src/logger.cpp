#include "logger.h"


#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextStream>

#include <iostream>

bool Logger::logEnabled = true;
int Logger::logLevel = 0;
QString Logger::logFilename = "w3c-log.log";
bool Logger::logToFile = true;

void Logger::log(LogLevel logType, QString source, QString message, QString file, int lineNr, QString function)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    if (logType == LogLevel::L_TRACE && logType >= logLevel )
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Trace    ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_DEBUG && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Debug    ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_INFO && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Info     ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_WARNING && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Warning  ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_CRITICAL && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Critical ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_FATAL && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Fatal    ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
}
void Logger::writeLogToFile(QString message)
{
    static QFile file("logfile.log");

    if(!file.isOpen()) { file.open(QIODevice::Append); }
    QTextStream stream(&file);

    stream << message;// << endl;
    stream.flush();
}
