#include <iostream>
#include <unistd.h>

int main()
{
    char buf[80];
    getcwd(buf, sizeof(buf));
    std::cout << "cur: " << buf << std::endl;
}