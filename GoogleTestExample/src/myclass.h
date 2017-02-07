#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>

class MyClass
{
public:
    MyClass();

    std::string appendString(const std::string& inString, const std::string& append);
    std::string appendNumber(const std::string& inString, int number);
};

#endif // MYCLASS_H
