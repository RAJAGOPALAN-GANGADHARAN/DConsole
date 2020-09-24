#include "typedefs.h"

class DConsole
{
    private:
        Platform_GUI window_handle;
    public:
        virtual void DConsoleApp() = 0;
};
