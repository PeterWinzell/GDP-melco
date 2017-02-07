#include "myclass2.h"

#include <string>

MyClass2::MyClass2()
{

}

std::string MyClass2::appendString(const std::string &inString, const std::string &append)
{
    return inString + append;
}

std::string MyClass2::appendNumber(const std::string &inString, int number)
{
    return inString + std::to_string(number);
}
