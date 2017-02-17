#ifndef MYCLASS2_H
#define MYCLASS2_H

#include <QObject>

class MyClass2
{
public:
    MyClass2();

    std::string appendString(const std::string& inString, const std::string& append);
    std::string appendNumber(const std::string& inString, int number);
};

#endif // MYCLASS2_H
