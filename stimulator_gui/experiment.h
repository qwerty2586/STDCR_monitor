#ifndef STDCR_MONITOR_EXPERIMENT_H
#define STDCR_MONITOR_EXPERIMENT_H

#include <QWidget>
#include <QString>
#include <stimulator_comm/stimulator.h>

/**
 * Interface Experiment, predstavuje abtraktni tridu widgetu,
 * ktery se bude vykraslovat v hlavnim okne dynamicky, zpocatku v nem umisten nebude
 * a bude se umistovat do okna jako celek az v pripade zmacknuti tlacitka v menu ci podobne.
 * Predpoklada se vice potomku teto tridy. Trida obsahuje jak uzivatelske rozhrani, tak
 * samotnou logiku experimentu - odeslani dat, prijem dat, ulozeni nastaveni a tak podobne
 *
 */

class Experiment : public QWidget {
Q_OBJECT

public:


    explicit Experiment(QWidget *parent = 0);

    static const int NO_EXPERIMENT = -1;

    /// poviny override, pomoci ktereho urcime zda je impementace tridy hotova
    virtual bool isImplemented() = 0;

    /// poviny override, vrati jmeno tridy
    virtual QString getName() = 0;

    /// pripoji komunikacni tridu
    virtual void setStimulator(Stimulator *port) { this->port = port; };

    /// prikaz ke spusteni ci vypnuti experimentu
    virtual void changeExperimentState(bool state) { };

    /// spojeni s komunikacni tridou
    Stimulator *port;
    
public slots:
    virtual void enterExperiment() {} ;
    virtual void leaveExperiment() {} ;

signals:

    /// vysila signal o uspesnem spusteni ci ukonceni experimentu
    void experimentStateChanged(bool);


private:


};


#endif //STDCR_MONITOR_EXPERIMENT_H
