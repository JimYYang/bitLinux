#include <stdio.h>
#include <unistd.h>

int main()
{
    /* printf("hello makefile"); */
    /* sleep(2); */
    int cnt = 10;
    while (cnt)
    {
        printf("%2d\r", cnt);
        fflush(stdout);
        sleep(1);
        cnt--;
    }
    return 0;
}
