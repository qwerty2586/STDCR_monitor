

#ifndef STDCR_MONITOR_MAIN_WINDOW_H
#define STDCR_MONITOR_MAIN_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <stdcr_comm/stimulator.h>
#include <stdcr_monitor/experiment.h>



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



    bool portConnected, experimentRunning;
    Stimulator *port;


    std::vector<Experiment *> experiments;

    int activeExperiment;
    int runningExperimentIndex;


    void initItems();

    void showMenu();

    void changeExperiment(const int experiment);

    void showExperiment(const int experiment);

    void clearLayout();


private slots:

    void menuButtonClick();

    void backClick();

    void startStopClick();

    void initExperiments();

    void portConnectDisconnectClick();

    void onPortConnected(bool connected);

    void onExperimentStateChanged(bool state);

    void debugpacket(char, QByteArray);

    void refreshPortList();


};


#endif //STDCR_MONITOR_MAIN_WINDOW_H
