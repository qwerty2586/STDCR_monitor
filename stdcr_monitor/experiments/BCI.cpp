

#include "BCI.h"
#include <QBoxLayout>
#include <QToolButton>


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
    f_tabs->addTab(new QWidget, QString::number(f_tabs->count()));

}

void BCI::clearFLeds() {
    f_tabs->clear();
}








