

#include "TEST_MODE.h"
#include <QVBoxLayout>
#include <QLabel>
#include "../params.h"



TEST_MODE::TEST_MODE(QWidget *parent) : Experiment(parent) {
    initItems();

};


void TEST_MODE::initItems() {

    QVBoxLayout *l = new QVBoxLayout();

    QGridLayout *enviroment_layout = new QGridLayout();
    QGridLayout *test_layout = new QGridLayout();

    l->addWidget(new QLabel("ENVIROMENT"), 0, Qt::AlignCenter);

    l->addLayout(enviroment_layout, 2);
    this->setLayout(l);

    enviroment_layout->setColumnStretch(0, 2);
    enviroment_layout->setColumnStretch(1, 1);
    enviroment_layout->setColumnStretch(2, 1);

    test_layout->setColumnStretch(0, 2);

    test_layout->setColumnStretch(1, 1);
    test_layout->setColumnStretch(2, 1);

    enviroment_layout->addWidget(new QLabel("TEMPERATURE"), 1, 0);
    temperature = new QLineEdit();
    temperature->setReadOnly(true);
    enviroment_layout->addWidget(temperature, 1, 1);
    enviroment_layout->addWidget(new QLabel(TEXT_CELSIUS), 1, 2);

    enviroment_layout->addWidget(new QLabel("VOLTAGE 3.3V"), 2, 0);
    voltage_3V = new QLineEdit();
    voltage_3V->setReadOnly(true);
    enviroment_layout->addWidget(voltage_3V, 2, 1);
    enviroment_layout->addWidget(new QLabel(TEXT_MV), 2, 2);

    enviroment_layout->addWidget(new QLabel("VOLTAGE 5.0V"), 3, 0);
    voltage_5V = new QLineEdit();
    voltage_5V->setReadOnly(true);
    enviroment_layout->addWidget(voltage_5V, 3, 1);
    enviroment_layout->addWidget(new QLabel(TEXT_MV), 3, 2);

    enviroment_layout->addWidget(new QLabel("VOLTAGE LED"), 4, 0);
    voltage_LED = new QLineEdit();
    voltage_LED->setReadOnly(true);
    enviroment_layout->addWidget(voltage_LED, 4, 1);
    enviroment_layout->addWidget(new QLabel(TEXT_MV), 4, 2);

    l->addWidget(new QLabel("TESTS"), 0, Qt::AlignCenter);

    l->addLayout(test_layout, 2);

    test_layout->addWidget(new QLabel("BRIGHTNESS"), 0, 0);
    brightness = new QSpinBox();
    brightness->setRange(MIN_P, MAX_P);
    test_layout->addWidget(brightness, 0, 1);
    test_layout->addWidget(new QLabel(TEXT_PERCENT), 0, 2);

    test_layout->addWidget(new QLabel("LED TEST"), 1, 0);
    led_test = new QComboBox();
    for(int i = 0; i < MAX_LEDS; i++){
        led_test->insertItem(i, QString::number(i));
    }
    test_layout->addWidget(led_test, 1, 1);

    test_layout->addWidget(new QLabel("SYNC OUT TEST"), 2, 0);
    sync_out_test = new QSpinBox();
    sync_out_test->setRange(MIN_MS, MAX_MS);
    test_layout->addWidget(sync_out_test, 2, 1);
    test_layout->addWidget(new QLabel(TEXT_MS), 2, 2);

}
