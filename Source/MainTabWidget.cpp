#include "MainTabWidget.h"
#include "utils.h"
#include <QtGui>
#include <QtWidgets>
#include <QTime>

MainTabWidget::MainTabWidget(QWidget* parent)
:QTabWidget(parent)
{

}

MainTabWidget::~MainTabWidget()
{

}

void MainTabWidget::CreateNewTab(QString tab_name,QString color,QString data)
{
    if (tabIndex.find(tab_name) == tabIndex.end())
    {
        QTableWidget *table = new QTableWidget(0,2,this);
        table->horizontalHeader()->hide();
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
        tabIndex[tab_name] = addTab(table, tab_name);
    }

    QLabel *timeStamp = new QLabel(QTime::currentTime().toString());
    QLabel *lineEdit1 = new QLabel(data);
    lineEdit1->setStyleSheet("QLabel { background-color: " + color + "; font-size: 15px; }");

    auto tabData = tabIndex[tab_name];
    QTableWidget *upWidget = (QTableWidget*)this->widget(tabData);
    int row = upWidget->rowCount();
    upWidget->insertRow(row);
    upWidget->setCellWidget(row, 0, timeStamp);
    upWidget->setCellWidget(row, 1, lineEdit1);
}
