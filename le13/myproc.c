#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    pid_t id = fork();

    if (id == 0)
    {
        int cnt = 5;
        while (cnt)
        {
            printf("child process: %d, parent id: %d, cnt: %d\n", getpid(), getppid(), cnt--);
            sleep(1);
            int *p = NULL;
            *p = 100;
        }
        exit(10);
    }
    /* pid_t res = wait(NULL); */
    int status = 0;
    pid_t res = waitpid(id, &status, 0);
    if (id > 0)
    {
        printf("wait success: %d, sig number: %d, child exit code: %d\n", res, status & 0x7F, (status >> 8) & 0xFF);
    }
    sleep(5);
}
