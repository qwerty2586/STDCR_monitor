#ifndef STDCR_MONITOR_EXPERIMENT_H
#define STDCR_MONITOR_EXPERIMENT_H

#include <QWidget>
#include <QString>


class Experiment : public QWidget {
Q_OBJECT

public:


    explicit Experiment(QWidget *parent = 0);

    static const int NO_EXPERIMENT = -1;

    virtual bool isImplemented() = 0;

    virtual QString getName() = 0;


private:


};

//Experiment::NAME = QString:"";

#endif //STDCR_MONITOR_EXPERIMENT_H
