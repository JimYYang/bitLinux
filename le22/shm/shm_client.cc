#include "comm.hpp"

int main()
{
    key_t k = getKey();
    printf("k: 0x%x\n", k);
    int shmid = getShm(k);
    printf("shmid: %d\n", shmid);

    
    char *start = (char*)attachShm(shmid);
    printf("attach success, address start: %p\n", start);

    const char *message = "hello server, 我是另一个进程, 正在和你通信";
    pid_t id = getpid();
    int cnt = 1;
    while (true)
    {
        sleep(1);
        snprintf(start, MAX_SIZE, "%s[pid:%d][消息编号:%d]", message, id, cnt++);
    }

    detachShm(start);

    return 0;
}