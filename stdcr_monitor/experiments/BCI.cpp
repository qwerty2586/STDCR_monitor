
#include "BCI.h"
#include <QLabel>

BCI::BCI(QWidget *parent) : Experiment(parent) {
    initItems();

};


void BCI::initItems() {
    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    tabs->addTab(new QWidget(), "F-VEP");
    tabs->addTab(new QWidget(), "T-VEP");
    tabs->addTab(new QWidget(), "C-VEP");
    l->addWidget(tabs);
    this->setLayout(l);

    // F-VEP



    // C-VEP
    QGridLayout *c_vepLayout = new QGridLayout();
    tabs->widget(2)->setLayout(c_vepLayout);

    c_vepLayout->setColumnStretch(0, 2);
    c_vepLayout->setColumnStretch(1, 1);
    c_vepLayout->setColumnStretch(2, 1);

    c_vepLayout->addWidget(new QLabel("Stimuli count"), 0, 0);
    stimuli_count = new QSpinBox();
    stimuli_count->setRange(0, 8);
    c_vepLayout->addWidget(stimuli_count, 0, 1);

    c_vepLayout->addWidget(new QLabel("Pattern"), 1, 0);
    pattern = new QCheckGrid(2, 16, 0);
    c_vepLayout->addWidget(pattern, 2, 0);

    c_vepLayout->addWidget(new QLabel("Target bin"), 3, 0);
    target_bin = new QLineEdit();
    target_bin->setText("0000 0000 0000 0000 0000 0000 0000 0000");
    target_bin->setFixedWidth(225);
    target_bin->setMaxLength(39);
    c_vepLayout->addWidget(target_bin, 4, 0);

    c_vepLayout->addWidget(new QLabel("Pulse length"), 5, 0);
    pulse_length = new QSpinBox();
    pulse_length->setRange(0, 30000);
    c_vepLayout->addWidget(pulse_length, 5, 1);
    c_vepLayout->addWidget(new QLabel("[ms]"), 5, 2);

    c_vepLayout->addWidget(new QLabel("Pulse skew"), 6, 0);
    pulse_skew = new QSpinBox();
    pulse_skew->setRange(0, 30000);
    c_vepLayout->addWidget(pulse_skew, 6, 1);
    c_vepLayout->addWidget(new QLabel("[ms]"), 6, 2);

    c_vepLayout->addWidget(new QLabel("Brightness"), 7, 0);
    brightness = new QSpinBox();
    brightness->setRange(0, 100);
    c_vepLayout->addWidget(brightness, 7, 1);
    c_vepLayout->addWidget(new QLabel("[%]"), 7, 2);


}
