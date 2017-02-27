

#ifndef STDCR_MONITOR_MAIN_WINDOW_H
#define STDCR_MONITOR_MAIN_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <stimulator_comm/stimulator.h>
#include <stimulator_gui/experiment.h>

/**
 * Hlavní okno
 * duraz je kladen hlavne, aby bylo mozne menit obsah layoutu okna, z menu na experiment
 * hlavni okno obsluhuje take udalosti tlaticka start stop pro zapnuti experimentu,
 * ktere jsou predavany experientu, experiment pak muze vyslat signal o uspesnem zapnuti,
 * na to hlavni okno reaguje zmenou stavu, a tlacika start na stop *
 */

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();


private:

    QVBoxLayout *windowLayout;
    std::vector<QPushButton *> menuButtons;
    QLabel *captionLabel, *experimentNameLabel;
    QPushButton *backButton, *startStopButton;
    QHBoxLayout *experimentButtonsLayout;
    QWidget *experimentButtonsWidget;
    QGroupBox *portGroup;
    QHBoxLayout *portLayout;
    QComboBox *portCombo;
    QPushButton *portConnectDisconnectButton, *portRefreshButton;


    /// stavove indikatory
    bool portConnected, experimentRunning;

    /// komunikacni trida
    Stimulator *port;

    /// seznam experimentu, v takovem poradi v jakem se zobrazi v menu
    std::vector<Experiment *> experiments;

    /// index zobrazeneho experimentu, pokud jsme v menu wpak Experiment::NO_EXPERIMENT
    int activeExperiment;

    /// index beziciho experimentu, pokud nebezi pak Experiment::NO_EXPERIMENT
    int runningExperimentIndex;

    /// iniciace prvku okna ktere nesouvisi s experimenty
    void initItems();

    /// iniciace seznamu experimentu a prirazeni komunikacni tridy stimulator
    void initExperiments();

    /// zobrazi prvky menu
    void showMenu();

    /// zmeni zobrazeni okna Experiment::NO_EXPERIMENT znamena zobrazit menu
    void changeExperiment(const int experiment);

    /// skryje menu a zonrazi
    void showExperiment(const int experiment);

    /// vycisti mrizku layoutu - tedy vyhazi vsechny prvky
    void clearLayout();


private slots:

    void menuButtonClick();

    void backClick();

    void startStopClick();

    void portConnectDisconnectClick();

    void onPortConnected(bool connected);

    void onExperimentStateChanged(bool state);

    void debugpacket(char, QByteArray);

    void refreshPortList();
};


#endif //STDCR_MONITOR_MAIN_WINDOW_H
