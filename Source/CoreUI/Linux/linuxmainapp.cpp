#include "linuxmainapp.h"
#include <iostream>

LinuxMainApp::LinuxMainApp()
    : DConsoleUI()
{
    std::cout << "Console App";
}
void LinuxMainApp::StartApp()
{
    for (int i = 0; i < 100000; ++i)
    {
        std::cout << "Hello world";
    }
}