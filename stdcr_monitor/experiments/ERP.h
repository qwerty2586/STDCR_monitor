
#ifndef STDCR_MONITOR_ERP_H
#define STDCR_MONITOR_ERP_H

#include <stdcr_monitor/experiment.h>
#include <QSpinBox>
#include <QTabWidget>
#include <QRadioButton>
#include <QPushButton>
#include <stdcr_comm/stimulator.h>


class ERP : public Experiment {
Q_OBJECT

public:


    explicit ERP(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("ERP"); };

    bool isImplemented() override { return true; };

    void changeExperimentState(bool state);




private:
    QTabWidget *tabs;
    QSpinBox *q_out, *q_wait;
    QRadioButton *q_edge_up, *q_edge_down;
    QRadioButton *q_rand_none, *q_rand_plus, *q_rand_minus, *q_rand_plusminus;

    struct Led {
        QSpinBox *pulse_up, *pulse_down, *dist_value, *dist_delay, *brightness;
    };
    std::vector<Led *> leds;
    QPushButton *q_add_led, *q_clear_leds;



    void initItems();

private slots:

    void addLed();

    void clearLeds();

    void loadFile(QString filepathname);

    void saveFile(QString filepathname);



};

#endif //STDCR_MONITOR_ERP_H
