#include "myStdio.h"

FILE_ *fopen_(const char *path_name, const char *mode)
{
    int flag = 0;
    int defaultMode = 0666;
    if (strcmp(mode, "r") == 0)
    {
        flag |= O_RDONLY;
    }
    else if (strcmp(mode, "w") == 0)
    {
        flag |= (O_WRONLY | O_CREAT | O_TRUNC);
    }
    else if (strcmp(mode, "a") == 0)
    {
        flag |= (O_WRONLY | O_CREAT | O_APPEND);
    }
    else
    {

    }
    int fd = 0;
    if (flag & O_RDONLY) fd = open(path_name, flag);
    else fd = open(path_name, flag, defaultMode);
    if (fd < 0)
    {
        const char *err = strerror(errno);
        write(2, err, strlen(err));
        return NULL;
    }
    FILE_ *fp = (FILE_*)malloc(sizeof(FILE_));
    assert(fp);

    fp->flags = SYNC_LINE;
    fp->fileno = fd;
    fp->cap = SIZE;
    fp->size = 0;
    memset(fp->buffer, 0, SIZE);

    return fp;
}

void fwrite_(const void *ptr, int num, FILE_ *fp)
{
    memcpy(fp->buffer + fp->size, ptr, num);
    fp->size += num;
    if (fp->flags & SYNC_NOW)
    {
        write(fp->fileno, fp->buffer, fp->size);
        fp->size = 0;
    }
    else if (fp->flags & SYNC_FULL)
    {
        if (fp->size == fp->cap)
        {
            write(fp->fileno, fp->buffer, fp->size);
            fp->size = 0;
        }
    }
    else if (fp->flags & SYNC_LINE)
    {
        if (fp->buffer[fp->size - 1] == '\n')
        {
            write(fp->fileno, fp->buffer, fp->size);
            fp->size = 0;
        }
    }
}

void fflush_(FILE_ *fp)
{
    if (fp->size > 0)
        write(fp->fileno, fp->buffer, fp->size);
    fsync(fp->fileno);
    fp->size = 0;
}

void fclose_(FILE_ *fp)
{
    fflush_(fp);
    close(fp->fileno);
}