#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QTextEdit>
#include <QGraphicsView>
#include <QLineEdit>
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


    QVBoxLayout *outputs_layout =  new QVBoxLayout();

    tabs->widget(0)->setLayout(outputs_layout);
    tabs->setStyleSheet("background: #cccccc");

    QGridLayout *test_grid = new QGridLayout();
    outputs_layout->addLayout(test_grid);


    QLabel *label = new QLabel("<b>OUTPUT 1</b>");
    test_grid->addWidget(label,0,0,3,1);
    test_grid->addWidget(new QRadioButton("LED"),0,1,1,1);
    test_grid->addWidget(new QRadioButton("AUDIO"),1,1,1,1);
    test_grid->addWidget(new QRadioButton("IMAGE"),2,1,1,1);
    test_grid->addWidget(new QLineEdit(),3,0,1,2);
    QGraphicsView *graphicsView = new QGraphicsView();
    graphicsView->setFixedSize(100,100);
    //graphicsView->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    test_grid->addWidget(graphicsView,0,2,4,1);







}
