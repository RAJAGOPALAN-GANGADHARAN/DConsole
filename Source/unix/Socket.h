#pragma once
#include "Source/IPC.h"
#include <sys/un.h>
#include <sys/stat.h>

class MainWindow;

class Socket:public IPC
{
    public:
        Socket();
        void create_server() override;
        void recieve_message_loop() override;
        ~Socket();

    private:
        sockaddr_un server_sock_address;
        fd_set fds, read_fds;
        int server_fd,fd_max;
};
