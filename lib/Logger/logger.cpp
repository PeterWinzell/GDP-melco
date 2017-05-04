#include "logger.h"


#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QTextStream>

#include <iostream>

Logger *Logger::instance = 0;
Logger::Logger(QObject *parent): QObject(parent)
{
}

Logger *Logger::getInstance()
{
  if (!instance)
      instance = new Logger();
  return instance;
}

void Logger::log(LogLevel logType, QString source, QString message, QString file, int lineNr, QString function)
{
    // Add these to output. I wan't to add a --verbose setting to enable/disable this extra information.
    Q_UNUSED(file);
    Q_UNUSED(lineNr);
    Q_UNUSED(function);

    if(!Logger::logEnabled) { return; }
    static QMutex mutex;
    QMutexLocker lock(&mutex);

    if (logType == LogLevel::L_TRACE && logType >= logLevel )
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Trace    ] : %3\n").arg(source.leftJustified(15, ' ',true), QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_DEBUG && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Debug    ] : %3\n").arg(source.leftJustified(15, ' ', true), QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_INFO && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Info     ] : %3\n").arg(source.leftJustified(15, ' ',true), QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_WARNING && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Warning  ] : %3\n").arg(source.leftJustified(15, ' ',true), QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_CRITICAL && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Critical ] : %3\n").arg(source.leftJustified(15, ' ',true), QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
    else if (logType == LogLevel::L_FATAL && logType >=  logLevel)
    {
        QString msg = QString("[ %1 ] [ %2 ] [ Fatal    ] : %3\n").arg(source.leftJustified(15, ' ',true), QDateTime::currentDateTime().toString("yyyy-MM-dd : hh:mm:ss.zzz"),message);
        std::cerr << qPrintable(msg);
        if(logToFile) { writeLogToFile(msg); }
    }
}

void Logger::writeLogToFile(QString message)
{
    static QFile file(Logger::logFilename);

    if(!file.isOpen()) { file.open(QIODevice::Append); }
    QTextStream stream(&file);

    stream << message;// << endl;
    stream.flush();
}
