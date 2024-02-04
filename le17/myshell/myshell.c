#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define NUM 1024
#define OPT_NUM 64
#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3
/* #define ERROR_REDIR 4 */

#define trimSpace(start) do{\
    while(isspace(*start)) start++;\
}while(0)
/* #define DEBUG */
char buffer[NUM];
char *myargv[OPT_NUM];
int lastCode = 0;
int lastSig = 0;

int redirType = NONE_REDIR;
char *redirFile = NULL;

void commandCheck(char *commands)
{
    assert(commands);
    char *start = commands;
    char *end = commands + strlen(commands);

    while (start < end)
    {
        if (*start == '>')
        {
            *start = '\0';
            start++;
            if (*start == '>')
            {
                redirType = APPEND_REDIR;
                start++;
            }
            else
            {
                redirType = OUTPUT_REDIR;
            }
            trimSpace(start);
            redirFile = start;
            break;
        }
        else if (*start == '<')
        {
            *start = '\0';
            start++;
            trimSpace(start);

            redirType = INPUT_REDIR;
            redirFile = start;
            break;
        }
        else 
        {
            start++;
        }
    }
}

int main()
{
    while (1)
    {
        redirType = NONE_REDIR;
        redirFile = NULL;
        errno = 0;

        printf("用户名@主机名 当前路径# ");
        fflush(stdout);

        char *s = fgets(buffer, sizeof(buffer) - 1, stdin);
        assert(s != NULL);
        (void)s;
        buffer[strlen(buffer) - 1] = 0;
        /* printf("test: %s\n", buffer); */

        commandCheck(buffer);

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
            switch (redirType) 
            {
                case NONE_REDIR:
                    break;
                case INPUT_REDIR:
                    {
                        int fd = open(redirFile, O_RDONLY);
                        if (fd < 0)
                        {
                            perror("open");
                            exit(errno);
                        }
                        dup2(fd, 0);
                    }
                    break;
                case OUTPUT_REDIR:
                case APPEND_REDIR:
                    {
                        umask(0);
                        int flags = O_WRONLY | O_CREAT;
                        if (redirType == APPEND_REDIR) flags |= O_APPEND;
                        else flags |= O_TRUNC;
                        int fd = open(redirFile, flags, 0666);
                        if (fd < 0)
                        {
                            perror("open");
                            exit(errno);
                        }
                        dup2(fd, 1);
                    }
                    break;
                default:
                    break;
            }
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
