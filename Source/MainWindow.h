#pragma once

#include <QtWidgets>
#include <QObject>
#include <thread>

struct Message;
class MainTabWidget;
class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();
    void UpdateMessage(Message);
    void closeEvent(QCloseEvent *) override;
    void closeIPC();
signals:
    void SignalCreateNewTab(QString, QString, QString);
private:
    void constructMenu();
    MainTabWidget *tabWidget;
    QObject *workerObject;
};
