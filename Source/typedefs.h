#pragma once
#if defined _WIN64
#include <windows.h>
using Platform_GUI = HWND;
#elif defined _WIN32
#include <windows.h>
using Platform_GUI = HWND;
#elif defined __APPLE__
#elif defined __linux__
using Platform_GUI = int;
#endif
