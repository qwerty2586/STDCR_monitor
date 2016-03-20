//
// Created by qwerty on 20. 3. 2016.
//

#ifndef STDCR_MONITOR_AUTOSTIMULATION_H
#define STDCR_MONITOR_AUTOSTIMULATION_H

#include "../experiment.h"

class AUTOSTIMULATION : public Experiment {
Q_OBJECT

public:


    explicit AUTOSTIMULATION(QWidget *parent = 0) { };

    QString getName() override { return QString::fromUtf8("AUTOSTIMULATION"); };

    bool isImplemented() override { return false; };


};

#endif //STDCR_MONITOR_AUTOSTIMULATION_H
