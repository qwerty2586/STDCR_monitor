//
// Created by Mike on 25.3.2016.
//

#ifndef STDCR_MONITOR_BIOSENSOR_LOGGER_H
#define STDCR_MONITOR_BIOSENSOR_LOGGER_H

#include "../experiment.h"

class BIOSENSOR_LOGGER : public Experiment {
    Q_OBJECT

public:

    explicit BIOSENSOR_LOGGER(QWidget *parent = 0) { };

    QString getName() override { return QString::fromUtf8("BIOSENSOR LOGGER"); };

    bool isImplemented() override { return false; };

};
#endif //STDCR_MONITOR_BIOSENSOR_LOGGER_H
