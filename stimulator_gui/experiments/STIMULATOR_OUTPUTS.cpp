#include <QTabWidget>
#include <QVBoxLayout>
#include "STIMULATOR_OUTPUTS.h"

STIMULATOR_OUTPUTS::STIMULATOR_OUTPUTS(QWidget *parent) : Experiment(parent) {

    initItems();
}

void STIMULATOR_OUTPUTS::initItems() {

    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    tabs->addTab(new QWidget(), "OUTPUTS SETUP");
    tabs->addTab(new QWidget(), "SCHEMA");
    l->addWidget(tabs);
    this->setLayout(l);



}
