#pragma once
#include "../platformmain.h"
#include <windows.h>

class WinMainApp:public DConsoleUI{
    public:
        WinMainApp();
        int WinMain(
            _In_ HINSTANCE hInstance,
            _In_opt_ HINSTANCE hPrevInstance,
            _In_ LPSTR lpCmdLine,
            _In_ int nCmdShow);
        void StartApp();
};