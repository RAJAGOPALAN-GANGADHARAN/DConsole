#pragma once

#include "../typedefs.h"
#include<stdio.h>
class DConsoleUI
{
    public:
        Platform_GUI platform_window;
        DConsoleUI(){
        };
        virtual void StartApp() = 0;
};
