#pragma once

#include <thread>
#include <QObject>
#include <QThread>
#include "utils.h"

class MainWindow;

class IPC : public QObject
{
    Q_OBJECT

public:
    IPC();
    virtual void create_server() = 0;
    virtual void recieve_message_loop() = 0;
    void register_window(MainWindow *);
    void setState(bool);
    void spawn_message_loop();
    void signal_finish();
    ~IPC();

protected:
    Message incoming_message;
    MainWindow *parent_window;
    bool appState;
    QThread workerThread;

signals:
    void signal_message_loop(IPC *);
};

class WorkerThread : public QThread
{
    Q_OBJECT
public slots:
    void ipc_listener(IPC* ipc_daemon)
    {
        ipc_daemon->recieve_message_loop();
    }

signals:
    void resultReady(const QString &result);
};

