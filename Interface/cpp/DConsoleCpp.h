#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/stat.h>

#include <string>
#include <iostream>

#define DCONSOLE_SERVER "/tmp/dconsole-server.socket"

struct Message
{
    char tab_name[16];
    char data[128];
};

void error_helper(std::string message, int code)
{
    std::cerr << message << std::endl;
    exit(code);
}

void debug_helper(std::string message)
{
#ifdef DEBUG
    std::cerr << message << std::endl;
#endif
}

int first_time_connect()
{
    int sock_fd = -1;
    if ((sock_fd = socket(AF_UNIX, SOCK_SEQPACKET, 0)) == -1)
        error_helper("socket creation failed", 1);

    sockaddr_un socket_address;

    memset(&socket_address, 0, sizeof(struct sockaddr_un));
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, DCONSOLE_SERVER, sizeof(socket_address.sun_path) - 1);

    if (connect(sock_fd, (const struct sockaddr *)&socket_address, sizeof(struct sockaddr_un)) == -1)
        debug_helper("connection with server failed");

    return sock_fd;
}

static int client_fd = -1;

void string_copy(char* field,std::string s,int size)
{
    memset(field, '\0', sizeof(field));
    int i = 0;
    for (i = 0; i < std::min((int)s.size(), size); ++i)
    {
        field[i] = s[i];
    }
}

void send_base(Message msg)
{
    int con_c = 0;
    while(write(client_fd,&msg,sizeof(msg))<0)
    {
        client_fd = first_time_connect();
        con_c++;
        if (con_c > 25)
            error_helper("Max Connection attempts failed", 1);
    }
}

void DConsoleSend(std::string console_msg,std::string tab_name="default")
{
    Message m;
    string_copy(m.data, console_msg,128);
    string_copy(m.tab_name, tab_name,16);
    send_base(m);
}

void DConsoleSend(int console_msg, std::string tab_name = "default")
{
    auto et = std::to_string(console_msg);
    Message m;
    string_copy(m.data, et, 128);
    string_copy(m.tab_name, tab_name, 16);
    send_base(m);
}
