#include "Sockets.h"
#include "MainWindow.h"
#include "utils.h"
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

// Driver code
Socket::Socket()
{
    server_fd = -1;
}

Socket::~Socket()
{
    close(server_fd);
}

void Socket::create_server(MainWindow* window)
{
    struct stat statBuffer;
    if(stat(DCONSOLE_SERVER,&statBuffer) == 0)
    {
        if(unlink(DCONSOLE_SERVER) == -1)
        {
            exit_helper("stat-unlink Failed", FAIL_CODE);
        }
    }

    if((server_fd=socket(AF_UNIX,SOCK_SEQPACKET,0))<0)
    {
        exit_helper("server socket creation failed", FAIL_CODE);
    }

    server_sock_address.sun_family=AF_UNIX;
    strncpy(server_sock_address.sun_path, DCONSOLE_SERVER,
            sizeof(server_sock_address.sun_path) - 1);

    if(bind(server_fd,(const sockaddr*)&server_sock_address,
        (socklen_t)sizeof(server_sock_address)) == -1)
    {
        exit_helper("Server Bind failed", FAIL_CODE);
    }

    if(listen(server_fd, 10) == -1)
    {
        exit_helper("Server Listen failed", FAIL_CODE);
    }


    //FD Events subscription
    FD_ZERO(&fds);
    FD_SET(server_fd, &fds);
    fd_max = server_fd;

    debug_helper("Started the server");
    parent_window = window;
    debug_helper("Attached window to socket events");
}

void Socket::recieve_message_loop()
{
    bool state = true;
    while (state)
    {
        read_fds = fds;

        if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) == -1)
            exit_helper("Select blocking signal failed",FAIL_CODE);

        for (int fd = 0; fd < (fd_max + 1); fd++)
        {
            if (FD_ISSET(fd, &read_fds))
            {
                if (fd == server_fd)
                {
                    int fd_new;
                    if ((fd_new = accept(server_fd, NULL, NULL)) == -1)
                        exit_helper("Accept failed",FAIL_CODE);

                    FD_SET(fd_new, &fds);
                    if (fd_new > fd_max)
                        fd_max = fd_new;
                    debug_helper("Connected a new application");
                }
                else
                {
                    Message msg;
                    ssize_t numbytes = read(fd, &msg, sizeof(msg));
                    if(numbytes == -1)
                        exit_helper("Read Error", FAIL_CODE);
                    else if (numbytes == 0)
                    {
                        debug_helper(std::string("Socket ")+ std::to_string(fd) +std::string(" closed by client"));
                        if (close(fd) == -1)
                            exit_helper("close",FAIL_CODE);
                        FD_CLR(fd, &fds);
                    }
                    else
                    {
                        debug_helper("recieved a message:");
                        debug_helper(msg.data);
                        debug_helper(msg.tab_name);
                        parent_window->UpdateMessage(msg);
                    }
                }
            }
        }
    }
}
