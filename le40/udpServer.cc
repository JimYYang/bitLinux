#include "udpServer.hpp"
#include <memory>
#include <unordered_map>
#include <fstream>
#include <signal.h>

using namespace Server;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " local_port\n\n";
}

const std::string dictTxt = "./dict.txt";

unordered_map<string, string> dict;

static bool cutString(const string& line, string&key, string& value, const string &sep)
{
    auto pos = line.find(sep);
    if (pos == string::npos)
    {
        return false;
    }
    key = line.substr(0, pos);
    value = line.substr(pos + sep.size());
    return true;
}

static void initDict()
{
    ifstream in(dictTxt, std::ios::binary);
    if (!in.is_open())
    {
        cerr << "open file" << dictTxt << " error." << endl;
        exit(OPEN_ERR);
    }
    string line;
    string key, value;
    while (getline(in, line))
    {
        if (cutString(line, key, value, ":"))
        {
            dict.insert(make_pair(key, value));
        }
    }
    in.close();
    cout << "load file successfully!" << endl;
}

void reload(int signo)
{
    (void)signo;
    initDict();
}

static void debugPrint()
{
    for (auto &[k, v] : dict)
    {
        cout << k << ": " << v << endl;
    }
}

void handlerMessage(int sockfd, string clientip, uint16_t clientport, string message)
{
    string response_message;
    auto iter = dict.find(message);
    if (iter == dict.end())
    {
        response_message = "unknown";
    }
    else
    {
        response_message = iter->second;
    }


    // 开始返回
    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(clientport);
    client.sin_addr.s_addr = inet_addr(clientip.c_str());

    sendto(sockfd, response_message.c_str(), response_message.size(), 0, (struct sockaddr*)&client, sizeof(client));
}

void execCommand(int sockfd, string clientip, uint16_t clientport, string cmd)
{
    if (cmd.find("rm") != string::npos || cmd.find("end") != string::npos || cmd.find("mv") != string::npos || cmd.find("rmdir") != string::npos || cmd.find("shutdown") != string::npos)
    {
        cerr << clientip << ": " << clientport << " 正在做一个非法的操作: " << cmd << endl;
        return;
    }
    
    string response;
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == nullptr)
    {
        response = cmd + " exec falied.";
    }
    char line[1024];
    while (fgets(line, sizeof(line), fp))
    {
        response += line;
    }
    pclose(fp);

    struct sockaddr_in client;
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(clientport);
    client.sin_addr.s_addr = inet_addr(clientip.c_str());

    sendto(sockfd, response.c_str(), response.size(), 0, (struct sockaddr*)&client, sizeof(client));
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    signal(2, reload);
    // initDict();
    /* debugPrint(); */
    uint16_t port = atoi(argv[1]);
    string ip = argv[1];
    // unique_ptr<udpServer> usvr(new udpServer(handlerMessage, port));
    unique_ptr<udpServer> usvr(new udpServer(execCommand, port));

    usvr->initServer();
    usvr->start();
    return 0;
}
