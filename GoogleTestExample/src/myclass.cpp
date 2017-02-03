#include "myclass.h"

#include <string>

MyClass::MyClass()
{

}

std::string MyClass::appendString(const std::string &inString, const std::string &append)
{
    return inString + append;
}

std::string MyClass::appendNumber(const std::string &inString, int number)
{
    return inString + std::to_string(number);
}
