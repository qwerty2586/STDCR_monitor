#ifndef STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H
#define STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H

#include <stimulator_gui/experiment.h>
#include <QString>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QGraphicsView>

class STIMULATOR_OUTPUTS : public Experiment {
Q_OBJECT

public:

    explicit STIMULATOR_OUTPUTS(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("STIMULATOR OUTPUTS"); };

    bool isImplemented() override { return true; };


private:

    void initItems();

    QTabWidget *tabs;

    struct Output {
        QLabel *label;
        QRadioButton *radio_led, *radio_image, *radio_audio;
        QLineEdit *path_line;
        QLabel *image;
    };
    std::vector<Output *> outputs;

    QPalette LINEEDIT_READONLY_PALLETE,LINEEDIT_PALLETE;

private slots:
    void outputs_changed(bool output_enable);
    
    void loadFile(QString filepathname);

    void saveFile(QString filepathname);


};


#endif //STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H
