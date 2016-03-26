

#include "TEST_MODE.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>

const static int MAX_LEDS = 8;

TEST_MODE::TEST_MODE(QWidget *parent) : Experiment(parent) {
    initItems();

};


void TEST_MODE::initItems() {

    QVBoxLayout *l = new QVBoxLayout();

    QGridLayout *enviroment_layout = new QGridLayout();
    QGridLayout *test_layout = new QGridLayout();

    l->addWidget(new QLabel("Enviroment"), 0, Qt::AlignCenter);

    l->addLayout(enviroment_layout, 2);
    this->setLayout(l);

    enviroment_layout->setColumnStretch(0, 2);
    enviroment_layout->setColumnStretch(1, 1);
    enviroment_layout->setColumnStretch(2, 1);

    test_layout->setColumnStretch(0, 2);
    test_layout->setColumnStretch(1, 1);
    test_layout->setColumnStretch(2, 1);

    enviroment_layout->addWidget(new QLabel("Temperature"), 1, 0);
    temperature = new QLineEdit();
    temperature->setDisabled(1);
    enviroment_layout->addWidget(temperature, 1, 1);
    enviroment_layout->addWidget(new QLabel("[C]"), 1, 2);

    enviroment_layout->addWidget(new QLabel("Voltage 3.3V"), 2, 0);
    voltage_3V = new QLineEdit();
    voltage_3V->setDisabled(1);
    enviroment_layout->addWidget(voltage_3V, 2, 1);
    enviroment_layout->addWidget(new QLabel("[mV]"), 2, 2);

    enviroment_layout->addWidget(new QLabel("Voltage 5.0V"), 3, 0);
    voltage_5V = new QLineEdit();
    voltage_5V->setDisabled(1);
    enviroment_layout->addWidget(voltage_5V, 3, 1);
    enviroment_layout->addWidget(new QLabel("[mV]"), 3, 2);

    enviroment_layout->addWidget(new QLabel("Voltage LED"), 4, 0);
    voltage_LED = new QLineEdit();
    voltage_LED->setDisabled(1);
    enviroment_layout->addWidget(voltage_LED, 4, 1);
    enviroment_layout->addWidget(new QLabel("[mV]"), 4, 2);

    l->addWidget(new QLabel("Tests"), 0, Qt::AlignCenter);

    l->addLayout(test_layout, 2);

    test_layout->addWidget(new QLabel("Brightness"), 0, 0);
    brightness = new QSpinBox();
    brightness->setRange(0, 100);
    test_layout->addWidget(brightness, 0, 1);
    test_layout->addWidget(new QLabel("[%]"), 0, 2);

    test_layout->addWidget(new QLabel("LED test"), 1, 0);
    led_test = new QComboBox();
    for(int i = 0; i < MAX_LEDS; i++){
        led_test->insertItem(i, QString::number(i));
    }
    test_layout->addWidget(led_test, 1, 1);

    test_layout->addWidget(new QLabel("Sync out test"), 2, 0);
    sync_out_test = new QSpinBox();
    sync_out_test->setRange(0, 30000);
    test_layout->addWidget(sync_out_test, 2, 1);
    test_layout->addWidget(new QLabel("[ms]"), 2, 2);

}
