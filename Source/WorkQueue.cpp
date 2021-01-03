#include "WorkQueue.h"
#include "utils.h"
#include "MainWindow.h"
#include <QObject>

WorkQueue::WorkQueue(QObject* parent):QObject(parent)
{
    //connect(this, SIGNAL(AddNewMessage()), parent,SLOT(MainWindow::UpdateMessage) );
}

WorkQueue::~WorkQueue()
{

}    
void WorkQueue::push(Message msg)
{
}
