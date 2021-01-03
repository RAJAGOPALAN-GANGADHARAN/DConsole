#include "MainWindow.h"
#include "MainTabWidget.h"
#include "utils.h"
#include <thread>
#include <QtWidgets>
#include <map>

MainWindow::MainWindow(QWidget* parent)
{
    this->setWindowTitle("DConsole");
    this->resize(300, 300);
    QWidget *centralWidget = new QWidget(this);

    // QTabWidget* tabs=new QTabWidget(centralWidget);
    // tabs->setFixedSize(245, 245);
    // tabs->addTab(new QWidget(),"TAB 1");
    // tabs->addTab(new QWidget(), "TAB 2");

    tabWidget = new MainTabWidget(centralWidget);
    tabWidget->setFixedSize(245, 245);
    this->setCentralWidget(centralWidget);
    //QListWidget
    connect(this, SIGNAL(SignalCreateNewTab(QString,QString,QString)),
            tabWidget, SLOT(CreateNewTab(QString,QString,QString)));
}
void MainWindow::UpdateMessage(Message msg)
{
    SignalCreateNewTab(msg.tab_name, msg.color, msg.data);
}
void MainWindow::set_reciever_thread(std::thread* thread)
{
    this->reciever_thread = thread;
}

MainWindow::~MainWindow()
{
    reciever_thread->join();
    //delete tabWidget;
}
