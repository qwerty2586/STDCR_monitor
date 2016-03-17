//
// Created by qwerty on 16. 3. 2016.
//

#ifndef STDCR_MONITOR_ERP_H
#define STDCR_MONITOR_ERP_H

#include "../experiment.h"

class ERP : public Experiment {
Q_OBJECT

public:
    explicit ERP(QWidget *parent = 0);

    QString getName() override;

    bool isImplemented();
};

//QString ERP::NAME = "ERP";

#endif //STDCR_MONITOR_ERP_H
