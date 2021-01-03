#include <QApplication>
#include "MainWindow.h"
#include "Sockets.h"

int main(int argc, char **argv)
{
    QApplication app(argc,argv);
    MainWindow* mw = new MainWindow();
    mw->show();

    Socket st;
    st.create_server(mw);
    auto message_thread = st.spawn_message_loop();
    
    // [WIP]Thread termination
    //mw.set_reciever_thread(&message_thread);

    return app.exec();
}
