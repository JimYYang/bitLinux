#include "tcpClient.hpp"
#include <memory>

using namespace std;


static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " server_ip server_port\n\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);

    unique_ptr<TcpClient> tcli(new TcpClient(server_ip, server_port));
    tcli->initClient();
    tcli->start();

    return 0;
}