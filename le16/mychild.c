#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define NUM 10
typedef void (*func_t)();

func_t handlertask[NUM];

void task1()
{
    printf("handler task1\n");
}
void task2()
{
    printf("handler task2\n");
}
void task3()
{
    printf("handler task3\n");
}

void loadTask()
{
    memset(handlertask, 0, sizeof(handlertask));
    handlertask[0] = task1;
    handlertask[1] = task2;
    handlertask[2] = task3;
}
int main()
{
    pid_t id = fork();
    assert(id != -1);
    if (id == 0)
    {
        int cnt = 5;
        while (cnt)
        {
            printf("child, pid: %d, ppid: %d, cnt: %d\n", getpid(), getppid(), cnt--);
            sleep(1);
        }
        exit(0);
    }
    // 非阻塞
    loadTask();
    int status = 0;
    while (1)
    {
        pid_t ret = waitpid(id, &status, WNOHANG); // 非阻塞 如果子进程没退出 父进程检测之后 立即返回
        if (ret == 0)
        {
            // 子进程没有退出 waitpid没有失败 仅仅检测到了子进程没有退出
            printf("wait done, but child is running\n");
            for (int i = 0; handlertask[i] != NULL; ++i)
            {
                handlertask[i]();
            }
        }
        else if (ret == id)
        {
            // waitpid调用成功 && 子进程推出了
            printf("wait success, exit code: %d, sig: %d\n", (status >> 8) & 0xFF, (status) & 0x7F);
            break;
        }
        else
        {
            // waitpid调用失败
            printf("waitpid call failed\n");
            break;
        }
        sleep(1);
    }



    // 让OS释放子进程的僵尸状态
    // 获取子进程的退出结果
    // 在等待期间  子进程没有退出的时候  父进程只能阻塞等待
    /* int status = 0; */
    /* int ret = waitpid(id, &status, 0); */
    /* if (ret > 0) */
    /* { */
    /*     if (WIFEXITED(status)) */
    /*     { */
    /*         printf("exit code: %d\n", WEXITSTATUS(status)); */
    /*     } */
    /*     /1* printf("wait success, exit code: %d, sig: %d\n", (status >> 8) & 0xFF, (status) & 0x7F); *1/ */
    /* } */

}
