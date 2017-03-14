#ifndef STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H
#define STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H

#include <QString>
#include <stimulator_gui/experiment.h>

class STIMULATOR_OUTPUTS : public Experiment {
    Q_OBJECT

public:

    explicit STIMULATOR_OUTPUTS(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("STIMULATOR OUTPUTS"); };

    bool isImplemented() override { return true; };


private:

    void initItems();

    QTabWidget *tabs;

};


#endif //STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H
