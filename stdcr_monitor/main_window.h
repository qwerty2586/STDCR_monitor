

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
    QLabel *captionLabel;
    int activeExperiment;

    void initMenuItems();

    void showMenu();

    void hideMenu();



public slots:

    void menuButtonClick();

    void changeExpreriment(const int experiment);
};


#endif //STDCR_MONITOR_MAIN_WINDOW_H
