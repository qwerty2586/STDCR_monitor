
#include "ERP.h"
#include <QVBoxLayout>
#include <QLabel>


const static QString ERP_MS = "[ms]";
const static QString ERP_OUT = "OUT";
const static QString ERP_WAIT = "WAIT";
const static QString ERP_EDGE = "EDGE";
const static QString ERP_RAND = "RAND";

const static int MAX_LEDS = 8;




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


    // 1st tab
    QGridLayout *syncSetupLayout = new QGridLayout();
    tabs->widget(0)->setLayout(syncSetupLayout);

    syncSetupLayout->setColumnStretch(0, 2);
    syncSetupLayout->setColumnStretch(1, 1);
    syncSetupLayout->setColumnStretch(2, 1);

    syncSetupLayout->addWidget(new QLabel(ERP_OUT), 0, 0);
    q_out = new QSpinBox();
    q_out->setRange(0, 30000);
    syncSetupLayout->addWidget(q_out, 0, 1);
    syncSetupLayout->addWidget(new QLabel(ERP_MS), 0, 2);

    syncSetupLayout->addWidget(new QLabel(ERP_WAIT), 1, 0);
    q_wait = new QSpinBox();
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

    // 2nd tab

    QGridLayout *ledSetupLayout = new QGridLayout();
    tabs->widget(1)->setLayout(ledSetupLayout);

    ledSetupLayout->setColumnStretch(0, 1);
    ledSetupLayout->setColumnStretch(1, 2);
    ledSetupLayout->setColumnStretch(2, 2);
    ledSetupLayout->setColumnStretch(3, 2);

    ledSetupLayout->addWidget(new QLabel("PULSE [ms]"), 0, 1);
    ledSetupLayout->addWidget(new QLabel("DISTRIBUTION [ms]"), 0, 2);
    ledSetupLayout->addWidget(new QLabel("BRIGHTNESS"), 0, 3);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("UP"));
    qhBoxLayout->addWidget(new QLabel("DOWN"));
    ledSetupLayout->addLayout(qhBoxLayout, 1, 1);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("VALUE"));
    qhBoxLayout->addWidget(new QLabel("DELAY"));
    ledSetupLayout->addLayout(qhBoxLayout, 1, 2);

    ledSetupLayout->addWidget(new QLabel("[%]"), 1, 3);

    q_add_led = new QPushButton("+");
    ledSetupLayout->addWidget(q_add_led, 2, 0);
    connect(q_add_led, SIGNAL(released()), this, SLOT(addLed()));

    q_clear_leds = new QPushButton("CLEAR");
    ledSetupLayout->addWidget(q_clear_leds, 0, 0);
    connect(q_clear_leds, SIGNAL(released()), this, SLOT(clearLeds()));
    connect(q_clear_leds, SIGNAL(released()), this, SLOT(addLed())); //a pridame prvni led
    q_clear_leds->hide();

    addLed(); // prepare first led


}

void ERP::addLed() {
    int row = leds.size();

    QGridLayout *ledSetupLayout = (QGridLayout *) tabs->widget(1)->layout();

    q_add_led->hide();

    ledSetupLayout->removeWidget(q_add_led);
    ledSetupLayout->addWidget(new QLabel(QString::fromUtf8("LED") + QString::number(row)), row + 2, 0);

    leds.push_back(new Led());
    leds[row]->pulse_up = new QSpinBox();
    leds[row]->pulse_down = new QSpinBox();
    leds[row]->dist_value = new QSpinBox();
    leds[row]->dist_delay = new QSpinBox();
    leds[row]->brightness = new QSpinBox();

    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(leds[row]->pulse_up);
    qhBoxLayout->addWidget(leds[row]->pulse_down);
    ledSetupLayout->addLayout(qhBoxLayout, row + 2, 1);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(leds[row]->dist_value);
    qhBoxLayout->addWidget(leds[row]->dist_delay);
    ledSetupLayout->addLayout(qhBoxLayout, row + 2, 2);

    ledSetupLayout->addWidget(leds[row]->brightness, row + 2, 3);

    // add + button back
    if (row < MAX_LEDS - 1) {
        ledSetupLayout->addWidget(q_add_led, row + 3, 0);
        q_add_led->show();
    }

    // show clear button
    if (row > 0) {
        q_clear_leds->show();
    }
}


void ERP::clearLeds() {
    QGridLayout *ledSetupLayout = (QGridLayout *) tabs->widget(1)->layout();
    ledSetupLayout->removeWidget(q_add_led);


    for (int i = 0; i < leds.size(); i++) {

        delete ledSetupLayout->itemAtPosition(i + 2, 0)->widget();
        delete ledSetupLayout->itemAtPosition(i + 2, 1)->layout();
        delete ledSetupLayout->itemAtPosition(i + 2, 2)->layout();

        delete leds[i]->pulse_up;
        delete leds[i]->pulse_down;
        delete leds[i]->dist_value;
        delete leds[i]->dist_delay;

        delete leds[i]->brightness;

    }
    leds.clear();
    ledSetupLayout->addWidget(q_add_led, 2, 0);

    q_clear_leds->hide(); // hide button for clearing leds

}
