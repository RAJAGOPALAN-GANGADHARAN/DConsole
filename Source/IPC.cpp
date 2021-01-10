#include "IPC.h"
#include "MainWindow.h"
#include "utils.h"
#include <QThread>
#include <unistd.h>


// Driver code
IPC::IPC()
{
    appState = true;
    WorkerThread *worker = new WorkerThread();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, this, &IPC::signal_finish);
    connect(this, &IPC::signal_message_loop, worker, &WorkerThread::ipc_listener);
    workerThread.start();
}

IPC::~IPC()
{
}

void IPC::register_window(MainWindow* window)
{
    parent_window = window;
    debug_helper("Attached window to socket events");
}

void IPC::spawn_message_loop()
{
    emit signal_message_loop(this);
}

void IPC::signal_finish()
{
    qDebug() << "Signal finished";
}

void IPC::setState(bool state)
{
    appState = state;
}
