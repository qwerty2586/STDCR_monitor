
#include "ERP.h"
#include <QVBoxLayout>
#include <QLabel>


const static QString ERP_MS = "[ms]";
const static QString ERP_QUESTION = "[?]";
const static QString ERP_OUT = "OUT";
const static QString ERP_WAIT = "WAIT";
const static QString ERP_EDGE = "EDGE";
const static QString ERP_RAND = "RAND";




ERP::ERP(QWidget *parent) : Experiment(parent) {
    initItems();


};


void ERP::initItems() {
    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    tabs->addTab(new QWidget(), "SYNC SETUP");
    tabs->addTab(new QWidget(), "LED SETUP");
    tabs->addTab(new QWidget(), "SCHEMA");
    l->addWidget(tabs);
    this->setLayout(l);


    // first tab
    QGridLayout *syncSetupLayout = new QGridLayout();
    tabs->widget(0)->setLayout(syncSetupLayout);

    syncSetupLayout->setColumnStretch(0, 2);
    syncSetupLayout->setColumnStretch(1, 1);
    syncSetupLayout->setColumnStretch(2, 1);

    syncSetupLayout->addWidget(new QLabel(ERP_OUT), 0, 0);
    q_out = new QSpinBox();
    q_out->setAccessibleName(ERP_OUT);
    q_out->setRange(0, 30000);
    syncSetupLayout->addWidget(q_out, 0, 1);
    syncSetupLayout->addWidget(new QLabel(ERP_QUESTION), 0, 2);

    syncSetupLayout->addWidget(new QLabel(ERP_WAIT), 1, 0);
    q_wait = new QSpinBox();
    q_wait->setAccessibleName(ERP_WAIT);
    q_wait->setRange(0, 30000);
    syncSetupLayout->addWidget(q_wait, 1, 1);
    syncSetupLayout->addWidget(new QLabel(ERP_MS), 1, 2);

    syncSetupLayout->addWidget(new QLabel(ERP_EDGE), 2, 0);
    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    q_edge_up = new QRadioButton("UP");
    q_edge_down = new QRadioButton("DOWN");
    qhBoxLayout->addWidget(q_edge_up);
    qhBoxLayout->addWidget(q_edge_down);
    syncSetupLayout->addLayout(qhBoxLayout, 2, 1, Qt::AlignLeft);

    syncSetupLayout->addWidget(new QLabel(ERP_RAND), 3, 0);
    qhBoxLayout = new QHBoxLayout();
    q_rand_none = new QRadioButton("0");
    q_rand_plus = new QRadioButton("+");
    q_rand_minus = new QRadioButton("-");
    q_rand_plusminus = new QRadioButton("+ -");
    qhBoxLayout->addWidget(q_rand_none);
    qhBoxLayout->addWidget(q_rand_plus);
    qhBoxLayout->addWidget(q_rand_minus);
    qhBoxLayout->addWidget(q_rand_plusminus);
    syncSetupLayout->addLayout(qhBoxLayout, 3, 1, Qt::AlignLeft);

}

