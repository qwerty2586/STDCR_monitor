
#ifndef STDCR_MONITOR_BCI_H
#define STDCR_MONITOR_BCI_H

#include "../experiment.h"
#include "../widgets/qcheckgrid.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QTabWidget>


class BCI : public Experiment{
    Q_OBJECT

public:

    explicit BCI(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("BCI"); };

    bool isImplemented() override { return true; };


private:


    QTabWidget *tabs;

    //C-VEP
    QCheckGrid *pattern;
    QLineEdit *target_bin;
    QSpinBox *stimuli_count, *pulse_length, *pulse_skew, *brightness;


    void initItems();


};

#endif //STDCR_MONITOR_BCI_H
