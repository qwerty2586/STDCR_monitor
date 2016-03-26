

#include "BCI.h"
#include <QBoxLayout>


BCI::BCI(QWidget *parent) : Experiment(parent) {
    initItems();
};

void BCI::initItems() {

    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    f_tabs = new QTabWidget();
    tabs->addTab(f_tabs, "F-VEP");
    tabs->addTab(new QWidget(), "T-VEP");
    tabs->addTab(new QWidget(), "C-VEP");
    l->addWidget(tabs);
    this->setLayout(l);

    // f-vep


    f_add_led = new QToolButton(f_tabs);
    f_add_led->setText("+");
    f_tabs->setCornerWidget(f_add_led, Qt::TopLeftCorner);
    f_clear_leds = new QToolButton(f_tabs);
    f_clear_leds->setText("CLEAR");
    f_tabs->setCornerWidget(f_clear_leds, Qt::TopRightCorner);

    connect(f_add_led, SIGNAL(released()), this, SLOT(addFLed()));
    connect(f_clear_leds, SIGNAL(released()), this, SLOT(clearFLeds()));
    connect(f_clear_leds, SIGNAL(released()), this, SLOT(addFLed()));

    addFLed();







    // c-vep

    QGridLayout *cvepLayout = new QGridLayout();
    tabs->widget(2)->setLayout(cvepLayout);

    cvepLayout->setColumnStretch(0, 2);
    cvepLayout->setColumnStretch(1, 1);
    cvepLayout->setColumnStretch(2, 1);

    cvepLayout->addWidget(new QLabel("STIMULI COUNT"), 0, 0);
    q_stimuli_count = new QSpinBox();
    cvepLayout->addWidget(q_stimuli_count, 0, 1);

    cvepLayout->addWidget(new QLabel("PATTERN"), 1, 0);
    q_pattern = new QCheckGrid(2, 16);
    cvepLayout->addWidget(q_pattern, 2, 0, 1, 3);  // zapotrebi roztahnout prez vicero sloupku
    connect(q_pattern, SIGNAL(valueChanged()), this, SLOT(patternChanged()));

    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("TARGET BIN      ")); // potrebujeme trosku odsadit
    q_target_bin = new QLabel("");
    qhBoxLayout->addWidget(q_target_bin);
    cvepLayout->addLayout(qhBoxLayout, 3, 0, 1, 3, Qt::AlignLeft);

    cvepLayout->addWidget(new QLabel("TARGET WAVEFORM"), 4, 0);
    q_target_wave = new QWaveForm(32);
    cvepLayout->addWidget(q_target_wave, 5, 0, 1, 3);

    patternChanged(); // nastavime target bin a target wave

    cvepLayout->addWidget(new QLabel("PULSE LENGTH"), 6, 0);
    q_pulse_length = new QSpinBox();
    cvepLayout->addWidget(q_pulse_length, 6, 1);
    cvepLayout->addWidget(new QLabel("[ms]"), 6, 2);

    cvepLayout->addWidget(new QLabel("PULSE SKEW"), 7, 0);
    q_pulse_skew = new QSpinBox();
    cvepLayout->addWidget(q_pulse_skew, 7, 1);
    cvepLayout->addWidget(new QLabel("[ms]"), 7, 2);

    cvepLayout->addWidget(new QLabel("BRIGHTNESS"), 8, 0);
    q_brightness = new QSpinBox();
    q_brightness->setRange(0, 100);
    cvepLayout->addWidget(q_brightness, 8, 1);
    cvepLayout->addWidget(new QLabel("[%]"), 8, 2);


}


void BCI::patternChanged() {
    std::vector<bool> pattern = q_pattern->value();
    QString *pattern_bin = new QString("");
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i]) pattern_bin->append("1");
        else pattern_bin->append("0");
        if ((i + 1) % 4 == 0) pattern_bin->append(" ");
    }
    q_target_bin->setText(pattern_bin->toAscii());

    q_target_wave->setValue(pattern);

}

void BCI::addFLed() {
    int index = fleds.size();

    f_tabs->addTab(new QWidget, QString::fromUtf8("LED") + QString::number(index));
    QGridLayout *fLedLayout = new QGridLayout();
    f_tabs->widget(index)->setLayout(fLedLayout);

    fLedLayout->setColumnStretch(0, 2);
    fLedLayout->setColumnStretch(1, 1);
    fLedLayout->setColumnStretch(2, 1);


    fleds.push_back(new FLed());

    FLed *l = fleds[index]; // zkraceni zapisu

    l->label1 = new QLabel("TIME ON");
    l->time_on = new QSpinBox();
    l->time_on->setRange(0, 30000);
    l->label1p = new QLabel("[ms]");
    fLedLayout->addWidget(l->label1, 0, 0);
    fLedLayout->addWidget(l->time_on, 0, 1);
    fLedLayout->addWidget(l->label1p, 0, 2);

    l->label2 = new QLabel("TIME OFF");
    l->time_off = new QSpinBox();
    l->time_off->setRange(0, 30000);
    l->label2p = new QLabel("[ms]");
    fLedLayout->addWidget(l->label2, 1, 0);
    fLedLayout->addWidget(l->time_off, 1, 1);
    fLedLayout->addWidget(l->label2p, 1, 2);

    l->label3 = new QLabel("FREQUENCY");
    l->frequency = new QSpinBox();
    l->frequency->setRange(0, 30000);
    l->label3p = new QLabel("[ms]");
    fLedLayout->addWidget(l->label3, 2, 0);
    fLedLayout->addWidget(l->frequency, 2, 1);
    fLedLayout->addWidget(l->label3p, 2, 2);

    l->label4 = new QLabel("DUTY CYCLE");
    l->duty_cycle = new QSpinBox();
    l->duty_cycle->setRange(0, 100);
    l->label4p = new QLabel("[%]");
    fLedLayout->addWidget(l->label4, 3, 0);
    fLedLayout->addWidget(l->duty_cycle, 3, 1);
    fLedLayout->addWidget(l->label4p, 3, 2);

    l->label5 = new QLabel("BRIGHTNESS");
    l->brightness = new QSpinBox();
    l->brightness->setRange(0, 100);
    l->label5p = new QLabel("[%]");
    fLedLayout->addWidget(l->label5, 4, 0);
    fLedLayout->addWidget(l->brightness, 4, 1);
    fLedLayout->addWidget(l->label5p, 4, 2);

}

void BCI::clearFLeds() {
    FLed *l;
    for (int i = 0; i < fleds.size(); ++i) {
        l = fleds[i];
        delete l->label1;
        delete l->label2;
        delete l->label3;
        delete l->label4;
        delete l->label5;

        delete l->label1p;
        delete l->label2p;
        delete l->label3p;
        delete l->label4p;
        delete l->label5p;

        delete l->time_on;
        delete l->time_off;
        delete l->frequency;
        delete l->duty_cycle;
        delete l->brightness;

        delete f_tabs->layout();
    }
    fleds.clear();
    f_tabs->clear();
}








