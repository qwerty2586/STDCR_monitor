#ifndef STDCR_MONITOR_BCI_H
#define STDCR_MONITOR_BCI_H

#include "../experiment.h"
#include <QTabWidget>
#include <QSpinBox>
#include <QLabel>
#include <QToolButton>
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


    };
    struct TLed {
        QCheckGrid *pattern;
        QCheckGrid *target_wave;

    };
    std::vector<FLed *> fleds;
    std::vector<TLed *> tleds;

    void initItems();

private slots:

    void addFLed();
    void clearFLeds();

    /*

     void addTLed();
     void clearTLeds();
 */
    void patternChanged();



};

#endif //STDCR_MONITOR_BCI_H
