#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>

#define NUM 1024
#define OPT_NUM 64
/* #define DEBUG */
char buffer[NUM];
char *myargv[OPT_NUM];
int lastCode = 0;
int lastSig = 0;

int main()
{
    while (1)
    {
        printf("用户名@主机名 当前路径# ");
        fflush(stdout);

        char *s = fgets(buffer, sizeof(buffer) - 1, stdin);
        assert(s != NULL);
        (void)s;
        buffer[strlen(buffer) - 1] = 0;
        /* printf("test: %s\n", buffer); */
        myargv[0] = strtok(buffer, " ");
        int i = 1;
        if (myargv[0] != NULL && strcmp(myargv[0], "ls") == 0)
        {
            myargv[i++] = "--color=auto";
        }
        while (myargv[i++] = strtok(NULL, " "));

        if (myargv[0] != NULL && strcmp(myargv[0], "cd") == 0)
        {
            if (myargv[1] != NULL)
            {
                chdir(myargv[1]);
            }
            continue;
        }

        if (myargv[0] != NULL && myargv[1] != NULL && strcmp(myargv[0], "echo") == 0)
        {
            if (strcmp(myargv[1], "$?") == 0) 
            {
                printf("%d, %d\n", lastCode, lastSig);
            }
            else 
            {
                printf("%s\n", myargv[1]);
            }
            continue;
        }

#ifdef DEBUG
        for (int i = 0; myargv[i]; i++)
        {
            printf("%s\n", myargv[i]);
        }
#endif
        pid_t id = fork();
        assert(id != -1);
        if (id == 0)
        {
            execvp(myargv[0], myargv);
            exit(1);
        }
        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        assert(ret > 0);
        (void)ret;
        lastCode = (status >> 8) & 0xFF;
        lastSig = status & 0x7F;
    }

    return 0;
}
