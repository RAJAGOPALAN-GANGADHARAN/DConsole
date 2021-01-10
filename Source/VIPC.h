#pragma once

#if defined _WIN64
#include "Source/win/WPipe.h"
using VIPC = WPipe;
#elif defined __linux__
#include "Source/unix/Socket.h"
using VIPC = Socket;
#endif