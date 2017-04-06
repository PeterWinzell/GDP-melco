#include "logger.h"


#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextStream>

#include <iostream>

int Logger::logLevel = 0;
bool Logger::logToFile = true;

void Logger::log(LogLevel logLevel, QString source, QString message, QString file, int lineNr, QString function)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    //qDebug() << file << lineNr << function;
    if (logLevel == LogLevel::L_TRACE)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Trace    ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        //std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logLevel == LogLevel::L_DEBUG)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Debug    ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logLevel == LogLevel::L_INFO)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Info     ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logLevel == LogLevel::L_WARNING)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Warning  ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logLevel == LogLevel::L_CRITICAL)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Critical ] : %3\n").arg(source, QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logLevel == LogLevel::L_FATAL)
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
