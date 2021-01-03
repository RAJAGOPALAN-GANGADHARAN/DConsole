#include "MainTabWidget.h"
#include "utils.h"
#include <QtWidgets>

MainTabWidget::MainTabWidget(QWidget* parent)
{

}

MainTabWidget::~MainTabWidget()
{

}

void MainTabWidget::CreateNewTab(QString tab_name,QString color,QString data)
{
    if (tabIndex.find(tab_name) == tabIndex.end())
    {
        QListWidget *listWidget = new QListWidget(this);
        tabIndex[tab_name] = {addTab(listWidget, tab_name),1};
    }

    // QListWidgetItem *newItem = new QListWidgetItem;
    // newItem->setText(data);
    // newItem->setBackgroundColor(QString(color));
    // auto tabData = tabIndex[tab_name];
    // QListWidget *upWidget = (QListWidget *)this->widget(tabData.first);
    // upWidget->insertItem(tabData.second, newItem);
    // tabIndex[tab_name].second++;
}