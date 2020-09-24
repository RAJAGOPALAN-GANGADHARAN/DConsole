#include "typedefs.h"
#include<iostream>

#if defined _WIN64
#include "CoreUI/Windows/winmainapp.h"
using MainApp = WinMainApp;
#elif defined __linux__
#include "CoreUI/Linux/linuxmainapp.h"
using MainApp = LinuxMainApp;
#endif

int main(int argc,char** argv){
    MainApp* app = new MainApp();
    app->StartApp();

    return 0;
}
