#include <stdio.h>
#include <string.h>

int main ()
{
    char src[150], dest[150];

    strcpy(src,  "/Users/vcvc/git/src/CPP_Test/test/getcwd");
    strcpy(dest, "/Source/");

    strncat(src, "/SSSS/", 18);

    printf("最终的目标字符串： %s", src);

    return(0);
}