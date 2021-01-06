#pragma once

#if defined __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstdio>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/select.h>
#include <sys/stat.h>

#define DCONSOLE_SERVER "/tmp/dconsole-server.socket"
static int client_fd = -1;

#elif defined _WIN64
#include <windows.h>
#include <stdio.h> 
#include <tchar.h>
HANDLE hPipe;
LPTSTR DCONSOLE_SERVER = TEXT("\\\\.\\pipe\\dconsole_pipe"); 
#endif 

#include <stdlib.h>
#include <string>
#include <iostream>


struct Message
{
    char tab_name[16];
    char color[16];
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
    #if defined __linux__
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
    #elif defined _WIN64
        while (1) 
        {
            hPipe = CreateFile( 
                DCONSOLE_SERVER,
                GENERIC_READ |
                GENERIC_WRITE, 
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL);
        
            if (hPipe != INVALID_HANDLE_VALUE) 
                break; 
        
            if (GetLastError() != ERROR_PIPE_BUSY) 
            {
                _tprintf( TEXT("Could not open pipe. GLE=%d\n"), GetLastError() ); 
                error_helper("Pipe Creation Failed",-1);
            }
        
            if (!WaitNamedPipe(DCONSOLE_SERVER, 20000)) 
            { 
                printf("Could not open pipe: 20 second wait timed out."); 
                return -1;
            } 
        }
        DWORD dwMode = PIPE_READMODE_MESSAGE; 
        BOOL fSuccess = SetNamedPipeHandleState( 
            hPipe,
            &dwMode,
            NULL,
            NULL);
        if (!fSuccess) 
        {
            _tprintf( TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError() ); 
            error_helper("Named Pipehandle State Failed",-1);
        }
    #endif
}


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
#if defined __linux__
    while(write(client_fd,&msg,sizeof(msg))<0)
    {
        client_fd = first_time_connect();
        con_c++;
        if (con_c > 25)
            error_helper("Max Connection attempts failed", 1);
    }
#elif defined _WIN64
    BOOL fSuccess = false;
    DWORD cbWritten;
    while(!fSuccess)
    {
        fSuccess = WriteFile( 
        hPipe,
        &msg,
        sizeof(Message),
        &cbWritten,
        NULL);

        if(!fSuccess)
        first_time_connect();

        con_c++;
        if (con_c > 25)
            error_helper("Max Connection attempts failed", 1);
    }
#endif
}

void DConsole(std::string console_msg,std::string color="white",
    std::string tab_name="default")
{
    Message m;
    string_copy(m.data, console_msg,128);
    string_copy(m.tab_name, tab_name,16);
    string_copy(m.color, color, 16);
    send_base(m);
}

void DConsole(int console_msg, std::string color = "white",
    std::string tab_name = "default")
{
    auto et = std::to_string(console_msg);
    Message m;
    string_copy(m.data, et, 128);
    string_copy(m.tab_name, tab_name, 16);
    string_copy(m.color, color, 16);
    send_base(m);
}

void DConsole(char* console_msg,char* color,char* tab_name)
{
    DConsole(std::string(console_msg), std::string(color), std::string(tab_name));
}
