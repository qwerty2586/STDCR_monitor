//
// Created by qwerty on 16. 3. 2016.
//

#ifndef STDCR_MONITOR_ERP_H
#define STDCR_MONITOR_ERP_H

#include "../experiment.h"
#include <QTabWidget>

class ERP : public Experiment {
Q_OBJECT

public:
    QTabWidget *tabs;

    explicit ERP(QWidget *parent = 0);

    QString getName() override;

    bool isImplemented();

private:

    void initItems();
};

#endif //STDCR_MONITOR_ERP_H
