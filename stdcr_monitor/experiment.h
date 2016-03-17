#ifndef STDCR_MONITOR_EXPERIMENT_H
#define STDCR_MONITOR_EXPERIMENT_H

#include <QWidget>


class Experiment : public QWidget {
Q_OBJECT

public:


    explicit Experiment(QWidget *parent = 0);

    static const int NO_EXPERIMENT = -1;
    static const QString EXPERIMENT_NAME;


private:


};

#endif //STDCR_MONITOR_EXPERIMENT_H
