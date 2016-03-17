
#ifndef STDCR_MONITOR_ERP_H
#define STDCR_MONITOR_ERP_H

#include "../experiment.h"
#include <QTabWidget>

class ERP : public Experiment {
Q_OBJECT

public:
    QTabWidget *tabs;

    explicit ERP(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("ERP"); };

    bool isImplemented() override { return true; };

private:

    void initItems();
};

#endif //STDCR_MONITOR_ERP_H
