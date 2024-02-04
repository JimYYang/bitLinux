#include <iostream>
#include <unistd.h>
#include <cassert>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main()
{
    int fds[2];
    int n = pipe(fds);
    assert(n == 0);

    pid_t id = fork();

    assert(id >= 0);
    if (id == 0)
    {
        close(fds[0]);
        // string msg = "hello, I am child.";
        const char *s = "I am child, I am sending msg to you.";
        int cnt = 0;
        while (true)
        {
            cnt++;
            char buffer[1024];
            snprintf(buffer, sizeof(buffer), "child->parent say: %s[%d][%d]\n", s, cnt, getpid());

            // 写端写满时阻塞 等待对方读取
            write(fds[1], buffer, strlen(buffer));
            sleep(1);
        }

        close(fds[1]);
        exit(0);
    }

    close(fds[1]);

    while (true)
    {
        char buffer[1024];
        // 如果管道中没了数据 默认会阻塞等待
        ssize_t s = read(fds[0], buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0;
            cout << "Get message# " << buffer << "| my pid: " << getpid() << endl;
        }
        else if (s == 0)
        {
            cout << "read: " << s << endl;
            break;
        }
    }
    n = waitpid(id, nullptr, 0);
    assert(n == id);
    close(fds[0]);

    return 0;
}