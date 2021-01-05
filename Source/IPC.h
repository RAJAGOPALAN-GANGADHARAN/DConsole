#pragma once
#include <thread>
#include "utils.h"

class MainWindow;

class IPC
{
    public:
        IPC();
        virtual void create_server() = 0;
        virtual void recieve_message_loop() = 0;
        std::thread spawn_message_loop() {
            return std::thread( [this] { recieve_message_loop(); } );
        }
        void register_window(MainWindow *);
        ~IPC();

    protected:
        Message incoming_message;
        MainWindow* parent_window;
};
