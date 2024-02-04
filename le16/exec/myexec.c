#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("process is running...\n");
    pid_t id = fork();
    assert(id != -1);
    extern char **environ;
    if (id == 0)
    {
        sleep(1);
        char* const env_[] = {
            (char*)"MYENV=123",
            NULL
        };
        putenv((char*)"MYENV=aaa");
        execvp(argv[1], (argv + 1));
        /* execle("./mybin", "./mybin", NULL, environ); */
        /* execlp("ls", "ls", "--color=auto", "-a", "-l", NULL); */
        /* char *const argv_[] = { */
        /*     "ls", */
        /*     "-a", */
        /*     "-l", */
        /*     NULL */
        /* }; */
        /* execv("/bin/ls", argv_); */
        exit(1);
    }
    int status = 0;
    pid_t ret = waitpid(id, &status, 0);
    if (ret == id)
    {
        printf("wait success, exit code: %d, sig: %d\n", (status >> 8) & 0xFF, (status) & 0x7F);
    }
    /* execl("/bin/ls", "ls", "--color=auto", "-a", "-l", NULL); */

    /* perror("execl"); */

    /* printf("process running done...\n"); */
    /* exit(1); */
}
