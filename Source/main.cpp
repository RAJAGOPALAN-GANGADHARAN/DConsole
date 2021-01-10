#include <QApplication>
#include "MainWindow.h"
#include "VIPC.h"

int main(int argc, char **argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // HiDPI pixmaps

    QApplication *app = new QApplication(argc, argv);

    MainWindow* mw = new MainWindow();
    mw->show();

    VIPC* ipc_daemon = new VIPC();
    ipc_daemon->register_window(mw);
    ipc_daemon->create_server();
    ipc_daemon->spawn_message_loop();

    return app->exec();
}
