#pragma once
#include <QObject>

struct Message;
class WorkQueue : public QObject
{
    Q_OBJECT
    public:
        explicit WorkQueue(QObject *parent = 0);
        ~WorkQueue();
        static void push(Message);
};
