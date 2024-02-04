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
typedef std::function<void()> func_t;
using namespace std;

void downloadTask()
{
    cout << getpid() << ": 下载任务\n" << endl;
    sleep(1);
}

void ioTask()
{
    cout << getpid() << ": io任务\n" << endl;
    sleep(1);
}

void flushTask()
{
    cout << getpid() <<": flush任务\n" << endl;
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

int recvTask(int readFd)
{
    int code = 0;
    ssize_t s = read(readFd, &code, sizeof(code));
    if (s == 4) return code;
    else if (s <= 0) return -1;
    else return 0;
}

void createSubProcess(vector<subEp> &subs, vector<func_t> &funcMap)
{
    vector<int> deleteFd;
    for (int i = 0; i < PROCESS_NUM; i++)
    {
        int fds[2];
        int n = pipe(fds);
        assert(n == 0);
        (void)n;
        pid_t id = fork();
        if (id == 0)
        {
            // 关闭前面的写端
            for (int i = 0; i < deleteFd.size(); i++)
            {
                close(deleteFd[i]);
            }
            // 子进程 进行处理任务
            close(fds[1]);
            while (true)
            {
                // 1. 获取命令码 如果没有发送 紫进程应该阻塞
                int commandCode = recvTask(fds[0]);
                // 2. 完成任务
                if (commandCode >= 0 && commandCode < funcMap.size())
                {
                    funcMap[commandCode]();
                }
                else if (commandCode == -1)
                {
                    break;
                }
            }
            exit(0);
        }
        close(fds[0]);
        subs.push_back(subEp(id, fds[1]));
        deleteFd.push_back(fds[1]);
    }

}

void sendTask(const subEp &process, int taskNum)
{
    cout << "send task num: " << taskNum << " send to -> " << process.name_ << endl;
    int n = write(process.writeFd_, &taskNum, sizeof(taskNum));
    assert(n == sizeof(int));
    (void)n;
}

void loadBlanceControl(const vector<subEp> &subs, const vector<func_t> &funcMap, int cnt)
{
    int processNum = subs.size();
    int taskNum = funcMap.size();

    bool quit = cnt == 0;
    while (true)
    {
        // 1. select son process
        int subIdx = rand() % processNum;

        // 2. select a task
        int taskIdx = rand() % taskNum;
        // 3. send task to process
        sendTask(subs[subIdx], taskIdx);

        sleep(1);

        if (!quit)
        {
            if (--cnt == 0)
                break;
        }
    }

    for (int i = 0; i < processNum; i++) close(subs[i].writeFd_);
}

void waitProcess(vector<subEp> &processes)
{
    int processNum = processes.size();
    for (int i = 0; i < processNum; i ++)
    {
        waitpid(processes[i].subId_, nullptr, 0);
        cout << "wait sub process success...: " << processes[i].subId_ << endl;
    }
}

int main()
{
    MAKESEED();
    // 1. 建立子进程和子进程通信的管道
    // 1.1 加载方法表
    vector<func_t> funcMap;
    loadTaskFunc(funcMap);

    // 1.2 创建子进程 并且维护好父子通信的信道
    vector<subEp> subs;
    createSubProcess(subs, funcMap);

    // 2. 这里是父进程 控制子进程 负载均衡地向子进程发送命令码
    int taskCnt = 3;
    loadBlanceControl(subs, funcMap, taskCnt);


    // 3. 回收子进程信息
    waitProcess(subs);
    return 0;
}