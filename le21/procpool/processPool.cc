#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <string>
#include <functional>
#include <cassert>
#include <ctime>

#define PROCESS_NUM 5

#define MAKESEED() srand((unsigned long)time(nullptr) ^ getpid() ^ 0x17237 ^ rand() % 1234)

typedef function<void()> func_t;
using namespace std;


void downloadTask()
{
    cout << "下载任务" << endl;
    sleep(1);
}

void ioTask()
{
    cout << "io任务" << endl;
    sleep(1);
}

void flushTask()
{
    cout << "flush任务" << endl;
    sleep(1);
}

void loadTaskFunc(vector<func_t> &out)
{
    assert(&out);
    out.push_back(downloadTask);
    out.push_back(ioTask);
    out.push_back(flushTask);
}


class subEp
{
public:
    subEp(pid_t subId, int writeFd)
    :subId_(subId), writeFd_(writeFd)
    {
        char nameBuffer[1024];
        snprintf(nameBuffer, sizeof(nameBuffer), "process-%d[pid(%d)-fd(%d)]", num++, subId_, writeFd_);
        name_ = nameBuffer;
    }
public:
    static int num;
    string name_;
    pid_t subId_;
    int writeFd_;

};
int subEp::num = 0;

void createSubProcess(vector<subEp> &subs, vector<func_t> &funcMap)
{
    for (int i = 0; i < PROCESS_NUM; i++)
    {
        int fds[2];
        int n = pipe(fds);
        assert(n == 0);
        (void)n;
        pid_t id = fork();
        if (id == 0)
        {
            close(fds[0]);
            exit(0);
        }
        close(fds[1]);
        subs.push_back(subEp(id, fds[1]));
    }

}

void sendTask(const subEp &process, int taskId)
{
    
}


int main()
{
    MAKESEED();
    vector<func_t> funcMap;
    loadTaskFunc(funcMap);

    vector<subEp> subs;
    createSubProcess(subs, funcMap);
    int processNum = subs.size();
    int taskNum = funcMap.size();

    bool quit = false;
    while (!quit)
    {
        // 1. select son process
        int subIdx = rand() % processNum;

        // 2. select a task
        int taskIdx = rand() % taskNum;
        // 3. send task to process
        sendTask(subs[subIdx], taskIdx);
    }

}