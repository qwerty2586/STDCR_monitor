#ifndef STDCR_MONITOR_BCI_H
#define STDCR_MONITOR_BCI_H

#include "../experiment.h"
#include <QTabWidget>
#include <QSpinBox>
#include <QLabel>
#include <QToolButton>
#include <QRadioButton>
#include "../widgets/qsaveloadwidget.h"
#include "../widgets/qwaveform.h"
#include "../widgets/qcheckgrid.h"

class BCI : public Experiment {
    Q_OBJECT

public:


    explicit BCI(QWidget *parent = 0);

    QString getName() override { return QString::fromUtf8("BCI"); };

    bool isImplemented() override { return true; };


private:
    QTabWidget *tabs;
    QSpinBox *q_stimuli_count, *q_pulse_length, *q_pulse_skew, *q_brightness;
    QCheckGrid *q_pattern;
    QWaveForm *q_target_wave;
    QLabel *q_target_bin;

    QTabWidget *f_tabs, *t_tabs;
    QToolButton *f_add_led, *f_clear_leds;
    QToolButton *t_add_led, *t_clear_leds;


    struct FLed {
        QLabel *label1, *label2, *label3, *label4, *label5;
        QLabel *label1p, *label2p, *label3p, *label4p, *label5p;
        QSpinBox *time_on, *time_off, *frequency, *duty_cycle, *brightness;
    };
    struct TLed {
        QLabel *label_pattern, *label_wave, *label_bin;
        QHBoxLayout *bin_layout;
        QCheckGrid *pattern;
        QWaveForm *waveform;
        QLabel *bin;
        QLabel *label1, *label2, *label3, *label4;
        QLabel *label2p, *label3p, *label4p;
        QSpinBox *pattern_length, *pulse_length, *pulse_skew, *brightness;

    };
    std::vector<FLed *> fleds;
    std::vector<TLed *> tleds;
    QSaveLoadWidget *q_save_load_widget;
    QRadioButton *q_scheme_radio_cvep, *q_scheme_radio_fvep, *q_scheme_radio_tvep;

    void initItems();

private slots:

    void addFLed();
    void clearFLeds();


    void addTLed();

    void clearTLeds();

    void tPatternChanged();

    void tLengthChanged();

    void patternChanged();

    void loadFile(QString filepathname);

    void saveFile(QString filepathname);

    void schemeRadioChanged();

    void tabChanged(int tab_index);


};

#endif //STDCR_MONITOR_BCI_H
