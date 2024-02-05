#include "comm.hpp"

int main()
{
    key_t k = getKey();
    printf("k: 0x%x\n", k);
    int shmid = createShm(k);
    printf("shmid: %d\n", shmid);

    sleep(5);

    char *start = (char*)attachShm(shmid);
    printf("attach success, address start: %p\n", start);

    sleep(5);

    while (true)
    {
        printf("client say: %s\n", start);
        sleep(1);
    }

    detachShm(start);

    sleep(10);

    delShm(shmid);
    return 0;
}