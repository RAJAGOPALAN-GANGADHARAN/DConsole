#pragma once

#include <QtWidgets>
#include <QObject>
#include <thread>

struct Message;
class MainWindow: public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();
    void UpdateMessage(Message);

    void set_reciever_thread(std::thread*);

    QListWidget *listWidget;
    int widget_row;
    std::thread* reciever_thread;
};
