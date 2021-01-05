#include <QApplication>
#include "MainWindow.h"
#include "IPC.h"

#if defined _WIN64
#include "Source/win/WPipe.h"
using VIPC = WPipe;
#elif defined __linux__
#include "Source/unix/Socket.h"
using VIPC = Socket;
#endif

int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // HiDPI pixmaps

    MainWindow* mw = new MainWindow();
    mw->show();

    VIPC* ipc_dameon = new VIPC();
    ipc_dameon->register_window(mw);
    ipc_dameon->create_server();

    auto message_thread = ipc_dameon->spawn_message_loop();
    
    // [WIP]Thread termination
    //mw.set_reciever_thread(&message_thread);

    return app.exec();
}
