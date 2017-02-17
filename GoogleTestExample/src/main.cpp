#include <iostream>
#include "myclass.h"

int main(int , char **)
{
    MyClass mc;

    std::string s = mc.appendString("Hello ", "World!");

    std::cout << s << std::endl;

    return 0;
}
