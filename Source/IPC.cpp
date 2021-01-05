#include "IPC.h"
#include "MainWindow.h"
#include "utils.h"


// Driver code
IPC::IPC()
{
}

IPC::~IPC()
{
}

void IPC::register_window(MainWindow* window)
{
    parent_window = window;
    debug_helper("Attached window to socket events");
}
