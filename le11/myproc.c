#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    /* while (1) */
    /* { */
    /*     /1* printf("hello\n"); *1/ */
    /*     printf("pid: %d, ppid: %d\n", getpid(), getppid()); */
    /*     sleep(1); */
    /* } */
    pid_t id = fork();
    printf("pid: %d, ppid: %d, id: %d\n", getpid(), getppid(), id);
    sleep(2);
    return 0;
}
