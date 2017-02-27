
#ifndef STDCR_MONITOR_TEST_MODE_H
#define STDCR_MONITOR_TEST_MODE_H

#include "../experiment.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>

class TEST_MODE : public Experiment {
    Q_OBJECT

public:

    explicit TEST_MODE(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("TEST MODE"); };

    bool isImplemented() override { return true; };


private:

    QLineEdit *temperature, *voltage_3V, *voltage_5V, *voltage_LED;
    QSpinBox *brightness, *sync_out_test;
    QComboBox *led_test;

    void initItems();

};

#endif //STDCR_MONITOR_TEST_MODE_H
