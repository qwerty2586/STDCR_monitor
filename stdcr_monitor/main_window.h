

#ifndef STDCR_MONITOR_MAIN_WINDOW_H
#define STDCR_MONITOR_MAIN_WINDOW_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QLabel>

#define EXPERIMENTS_COUNT 6
static const QString experiments[] = {"ERP", "BCI", "REACTION EXPERIMENT", "AUTOSTIMULATION", "BIOSENSOR LOGGER",
                                      "TEST MODE"};


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

};


#endif //STDCR_MONITOR_MAIN_WINDOW_H
