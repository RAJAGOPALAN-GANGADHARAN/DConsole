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
    QFont globalFont("Times", 15);
    this->setFont(globalFont);
    constructMenu();

    tabWidget = new MainTabWidget(this);
    this->setCentralWidget(tabWidget);
    connect(this, SIGNAL(SignalCreateNewTab(QString,QString,QString)),
            tabWidget, SLOT(CreateNewTab(QString,QString,QString)));
}

void MainWindow::constructMenu()
{
    menuBar()->addMenu("Menu");
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
    delete tabWidget;
}
