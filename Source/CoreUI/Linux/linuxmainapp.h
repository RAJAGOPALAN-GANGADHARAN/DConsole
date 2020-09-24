#pragma once
#include "../platformmain.h"

class LinuxMainApp:public DConsoleUI{
    public:
        LinuxMainApp();
        void StartApp() override;
};