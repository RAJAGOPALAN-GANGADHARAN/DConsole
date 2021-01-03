#pragma once

#include <QtWidgets>
#include <QObject>
#include <thread>

struct Message;
class MainTabWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();
    void UpdateMessage(Message);

    void set_reciever_thread(std::thread*);

    MainTabWidget *tabWidget;
    int widget_row;
    std::thread* reciever_thread;
signals:
    void SignalCreateNewTab(QString, QString, QString);
private:
    void constructMenu();
};
