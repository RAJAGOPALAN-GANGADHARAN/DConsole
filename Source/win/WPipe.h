#pragma once
#include "Source/IPC.h"
#include "Source/utils.h"
#include <windows.h>

class MainWindow;

struct ThreadInterface
{
    LPVOID lp;
    MainWindow* windowLoc;
};

class WPipe:public IPC
{
    public:
        WPipe();
        void create_server() override;
        void recieve_message_loop() override;
        ~WPipe();

    private:
        BOOL fConnected;
        HANDLE hPipe, hThread;
        LPCTSTR DCONSOLE_SERVER;
};
