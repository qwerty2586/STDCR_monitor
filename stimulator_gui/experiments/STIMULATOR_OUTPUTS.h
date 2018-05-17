#ifndef STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H
#define STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H

#include <stimulator_gui/experiment.h>
#include <QString>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QGraphicsView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <stimulator_gui/widgets/qclickablelabel.h>

class STIMULATOR_OUTPUTS : public Experiment {
Q_OBJECT

public:

    explicit STIMULATOR_OUTPUTS(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("STIMULATOR OUTPUTS"); };

    bool isImplemented() override { return true; };

public slots:
    void enterExperiment() override;
    void leaveExperiment() override;

private:

    void initItems();

    QTabWidget *tabs;

    struct Output {
        QLabel *label;
        QRadioButton *radio_led, *radio_image, *radio_audio;
        QLineEdit *path_line;
        QClickableLabel *image;
        QPushButton *load_button;
        bool previewing = false;
    };
    std::vector<Output *> outputs;

    struct Settings {
        QSpinBox *width,*height;
        QCheckBox *debug,*enabled;
        QRadioButton *fullscreen,*windowed;
        QPushButton *save;

    };

    Settings settings;

    QPalette LINEEDIT_READONLY_PALLETE,LINEEDIT_PALLETE;

private slots:
    void outputs_changed(bool output_enable);

    void load_file_clicked();

    void image_clicked();

    void edit_changed();
    
    void loadFile(QString filepathname);

    void saveFile(QString filepathname);

    void saveSettings();


};


#endif //STIMULATOR_CONTROL_STIMULATOR_OUTPUTS_H
