#pragma once

#include <iostream>
class DConsole : public std::streambuf {
    public:
    DConsole(){
        setp(0, 0);
    }
};