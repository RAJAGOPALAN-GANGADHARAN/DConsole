#pragma once
#include <sys/un.h>
#include <sys/stat.h>
#include <iostream>
#include <thread>
#include "utils.h"

class MainWindow;

class Socket
{
    public:
        Socket();
        void create_server(MainWindow*);
        void recieve_message_loop();
        std::thread spawn_message_loop() {
            return std::thread( [this] { recieve_message_loop(); } );
        }
        ~Socket();

    private:
        sockaddr_un server_sock_address;
        fd_set fds, read_fds;
        int server_fd,fd_max;
        Message incoming_message;
        MainWindow* parent_window;
};
