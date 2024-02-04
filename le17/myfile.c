#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define FILE_NAME(number) "a.txt"#number

int main()
{
    /* umask(0); */
    int fd = open("log.txt", O_RDONLY);
    /* int fd1 = open(FILE_NAME(1), O_WRONLY | O_CREAT | O_APPEND, 0666); */
    /* int fd2 = open(FILE_NAME(2), O_WRONLY | O_CREAT | O_APPEND, 0666); */
    /* int fd3 = open(FILE_NAME(3), O_WRONLY | O_CREAT | O_APPEND, 0666); */
    /* int fd4 = open(FILE_NAME(4), O_WRONLY | O_CREAT | O_APPEND, 0666); */
    // if (fd < 0)
    // {
    //     perror("open");
    //     return 1;
    // }
    // int cnt = 5;
    // char ouBuffer[64];
    // while (cnt)
    // {
    //     sprintf(ouBuffer, "%s:%d\n", "hellobit", cnt--);
    //     write(fd, ouBuffer, strlen(ouBuffer));
    // }
    dup2(fd, 0);

    char line[64];
    while (1)
    {
        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            break;
        }
        printf("%s", line);
    }

    /* printf("%d\n", fd); */
    /* fprintf(stdout, "openfd: %d\n", fd); */
    /* const char *msg = "123"; */
    /* fprintf(stdout, "%d\n", strlen(msg)); */

    fflush(stdout);
    close(fd);
    /* printf("%d\n", fd2); */
    /* printf("%d\n", fd3); */
    /* printf("%d\n", fd4); */

    /* close(fd0); */
    /* close(fd1); */
    /* close(fd2); */
    /* close(fd3); */
    /* close(fd4); */
}
