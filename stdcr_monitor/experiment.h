#ifndef STDCR_MONITOR_EXPERIMENT_H
#define STDCR_MONITOR_EXPERIMENT_H

#include <QWidget>
#include <QString>
#include <stdcr_comm/stimulator.h>


class Experiment : public QWidget {
Q_OBJECT

public:


    explicit Experiment(QWidget *parent = 0);

    static const int NO_EXPERIMENT = -1;

    virtual bool isImplemented() = 0;

    virtual QString getName() = 0;

    virtual void setStimulator(Stimulator *port) { this->port = port; };

    virtual void changeExperimentState(bool state) { };
    Stimulator *port;
signals:

    void experimentStateChanged(bool);


private:


};


#endif //STDCR_MONITOR_EXPERIMENT_H
