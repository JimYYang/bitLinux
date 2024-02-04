#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    /* pid_t id = fork(); */

    /* if (id < 0) */
    /* { */
    /*     perror("fork"); */
    /*     return 1; */
    /* } */
    /* else if (!id) */
    /* { */
    /*     while (1) */
    /*     { */
    /*         printf("child, pid: %d, ppid: %d\n", getpid(), getppid()); */
    /*     } */
    /* } */
    /* else */
    /* { */
    /*     while (1) */
    /*     { */
    /*         printf("parent, pid: %d, ppid: %d\n", getpid(), getppid()); */
    /*     } */
    /* } */
    /* while (1) */
    /* { */
    /*     int a = 1; */
    /*     printf("1\n"); */
    /*     sleep(1); */
    /* } */
    int a = 1;
    int*p1  = &a;
    int **p2 = &p1;
    printf("%d\n", p1);
    return 0;
}
