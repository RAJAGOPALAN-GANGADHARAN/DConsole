#pragma once

#include <iostream>
#include <string.h>
#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
    #endif
    #define socket_type SOCKET
    #include <winsock2.h>
    #include <Ws2tcpip.h>

    #pragma comment(lib, "ws2_32.lib")
#else
    /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
    #define socket_type int
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
    #include <unistd.h> /* Needed for close() */
#endif

struct dconsole_ostreambuf : public std::streambuf
{
    dconsole_ostreambuf(void* user_data = nullptr): 
    user_data_(user_data) 
    {
        sockInit();
        if(socketConnection()<0)
        {
            throw std::runtime_error("Failed to create socket for DConsole");
        }

    }
    ~dconsole_ostreambuf()
    {
        sockClose();
        sockQuit();
    }

protected:

    std::streamsize xsputn(const char_type* s, std::streamsize n) override 
    { 
        return handler(s, n, user_data_);
    };

    int_type overflow(int_type ch) override 
    {
        return handler(&ch, 1, user_data_);
    }

private:
    std::streamsize handler(const void* buf,std::streamsize sz,void* user_data)
    {
        if(sendData(reinterpret_cast<const char*>(buf),sz)<0)
            std::runtime_error("Couldn't send data to server via socket!");

        return sz;
    }
    int sockInit()
    {
    #ifdef _WIN32
        WSADATA wsa_data;
        return WSAStartup(MAKEWORD(1,1), &wsa_data);
    #else
        return 0;
    #endif
    }

    int socketConnection()
    {
        sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(8081);
        
    #ifdef _WIN32
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        connectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(connectionSocket == INVALID_SOCKET)
            return -1;
        std::cout << "Connecting";
        if(connect(connectionSocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        {
            std::cerr<<"Connection Failed"<<std::endl;
            return -1;
        }
    #else
        if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)<=0) 
        {
            std::cerr << "Invalid address/ Address not supported"<<std::endl;
            return -1;
        }
        connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(connectionSocket < 0)
            return -1;
        if (connect(connectionSocket, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            std::cerr<<"Connection Failed"<<std::endl;
            return -1;
        }
    #endif
        return 0;
    }

    int sendData(const char* rawBody, int size){
        char* payload = new char[2048];
        // snprintf(payload, 2048, "{\"tab\":\"cpp\",\"type\":1,\"body\":\"%s\"}", rawBody);
        // std::cerr<<payload<<std::endl;
        int len=strlen(payload);
        std::cerr << "Sending data" << std::endl;
        #ifdef _WIN32
        if(send(connectionSocket, payload, len, 0) == SOCKET_ERROR)
            return -1;
        #else
        if(send(connectionSocket, payload, len, 0) < 0)
            return -1;
        #endif
        return 0;
    }

    int sockClose()
    {

    int status = 0;

    #ifdef _WIN32
        status = shutdown(connectionSocket, SD_BOTH);
        if (status == 0) { status = closesocket(connectionSocket); }
    #else
        status = shutdown(connectionSocket, SHUT_RDWR);
        if (status == 0) { status = close(connectionSocket); }
    #endif

    return status;

    }
    int sockQuit()
    {
    #ifdef _WIN32
        return WSACleanup();
    #else
        return 0;
    #endif
    }
    
    void* user_data_;
    socket_type connectionSocket;
};

int DConsole() {
    dconsole_ostreambuf* dcBuff;
    try
    {
        dcBuff = new dconsole_ostreambuf();
    }
    catch(std::exception& e)
    {
        std::cout<<e.what()<<std::endl;
    }

    std::cout.rdbuf(dcBuff);
    return 0;
}
