

#ifndef STDCR_MONITOR_MAIN_WINDOW_H
#define STDCR_MONITOR_MAIN_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include "experiment.h"



class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);


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
    QPushButton *portConnectDisconnect;


    std::vector<Experiment *> experiments;

    int activeExperiment;

    void initItems();

    void showMenu();

    void changeExperiment(const int experiment);

    void showExperiment(const int experiment);

    void clearLayout();



public slots:

    void menuButtonClick();

    void backClick();

    void startStopClick();

    void initExperiments();
};


#endif //STDCR_MONITOR_MAIN_WINDOW_H
