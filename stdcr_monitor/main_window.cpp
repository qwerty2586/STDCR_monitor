

#include "main_window.h"
#include "params.h"
#include "experiment.h"


#include <QPushButton>
#include <QLabel>









MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    resize(MAIN_WIDTH, MAIN_HEIGHT);

    windowLayout = NULL;
    initMenuItems();
    windowLayout = new QVBoxLayout();

    activeExperiment = Experiment::NO_EXPERIMENT;
    showMenu();


};

void MainWindow::initMenuItems() {

    captionLabel = new QLabel(QString::fromUtf8("MAIN MENU"));

    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        menuButtons[i] = new QPushButton(experiments[i]);
        QObject::connect(menuButtons[i], SIGNAL(released()), this, SLOT(menuButtonClick()));
    }

}

void MainWindow::showMenu() {

    windowLayout->addWidget(this->captionLabel);


    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        windowLayout->addWidget(menuButtons[i]);

    }
    windowLayout->addStretch();
    this->setLayout(windowLayout);
}

void MainWindow::menuButtonClick() {
    QPushButton *sender = (QPushButton *) QObject::sender();
    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        if (sender == menuButtons[i])
            this->setWindowTitle(QString::number(i));

    }


}
