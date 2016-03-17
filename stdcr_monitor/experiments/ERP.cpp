
#include "ERP.h"
#include <QVBoxLayout>

ERP::ERP(QWidget *parent) : Experiment(parent) {
    initItems();


};


QString ERP::getName() {
    return QString::fromUtf8("ERP");
};


void ERP::initItems() {
    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    tabs->addTab(new QWidget(), "SYNC SETUP");
    tabs->addTab(new QWidget(), "LED SETUP");
    tabs->addTab(new QWidget(), "SCHEMA");
    l->addWidget(tabs);
    this->setLayout(l);


}
