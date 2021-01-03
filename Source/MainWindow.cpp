#include "MainWindow.h"
#include "utils.h"
#include<thread>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent)
:QWidget(parent)
{
    this->setWindowTitle("DConsole");
    this->resize(300, 300);
    listWidget = new QListWidget(this);
    widget_row = 1;
}

void MainWindow::UpdateMessage(Message msg)
{
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(msg.data);
    listWidget->insertItem(widget_row, newItem);
    widget_row++;
}
void MainWindow::set_reciever_thread(std::thread* thread)
{
    this->reciever_thread = thread;
}

MainWindow::~MainWindow()
{
    reciever_thread->join();
    delete listWidget;
}
