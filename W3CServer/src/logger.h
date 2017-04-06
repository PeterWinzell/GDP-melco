#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

enum LogLevel { L_TRACE, L_DEBUG, L_INFO, L_WARNING, L_CRITICAL, L_FATAL };

#define TRACE(source, message) Logger::log(LogLevel::L_TRACE, source, message, __FILE__, __LINE__, __FUNCTION__);
#define DEBUG(source, message) Logger::log(LogLevel::L_DEBUG, source, message, __FILE__, __LINE__, __FUNCTION__);
#define INFO(source, message) Logger::log(LogLevel::L_INFO, source, message, __FILE__, __LINE__, __FUNCTION__);
#define WARNING(source, message) Logger::log(LogLevel::L_WARNING, source, message, __FILE__, __LINE__, __FUNCTION__);
#define CRITICAL(source, message) Logger::log(LogLevel::L_CRITICAL, source, message, __FILE__, __LINE__, __FUNCTION__);
#define FATAL(source, message) Logger::log(LogLevel::L_FATAL, source, message, __FILE__, __LINE__, __FUNCTION__);

class Logger : public QObject
{
    Q_OBJECT
public:
    Logger(QObject *parent = Q_NULLPTR): QObject(parent) {}

    static void log(LogLevel logLevel, QString source, QString message, QString file, int lineNr, QString function);

    static int logLevel;
    static bool logToFile;
private:
    static void writeLogToFile(QString message);
};

#endif // LOGGER_H
