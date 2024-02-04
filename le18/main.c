#include "myStdio.h"

int main()
{
    FILE_ *fp = fopen_("./log.txt", "w");
    if (fp == NULL)
    {
        return 1;
    }

    const char *msg = "hello\n";

    fwrite_(msg, strlen(msg),fp);

    fclose_(fp);
}