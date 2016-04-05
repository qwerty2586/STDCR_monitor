
#ifndef STDCR_MONITOR_RESPONSE_EXPERIMENT_H
#define STDCR_MONITOR_RESPONSE_EXPERIMENT_H

#include "../experiment.h"
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QTabWidget>
#include <QLineEdit>

class RESPONSE_EXPERIMENT : public Experiment {
    Q_OBJECT

public:


    explicit RESPONSE_EXPERIMENT(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("RESPONSE EXPERIMENT"); };

    bool isImplemented() override { return true; };


private:

    QTabWidget *tabs;

    //TEST SETUP
    QSpinBox *led_count, *cycles, *wait_time_fixed, *wait_time_random, *miss_time, *brightness;
    QRadioButton *cont, *wait;
    QCheckBox *m, *f, *a, *w, *h;

    //TEST MODE
    QLineEdit *count_cycle, *last_time, *average_time, *miss_counter, *fail_counter;

    void initItems();

private slots:

    void loadFile(QString filepathname);

    void saveFile(QString filepathname);

};



#endif //STDCR_MONITOR_RESPONSE_EXPERIMENT_H
