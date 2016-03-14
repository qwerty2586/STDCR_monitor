#include "main_window.h"
#include "params.h"
#include "experiment.h"


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    resize(MAIN_WIDTH, MAIN_HEIGHT);

    windowLayout = NULL;
    initMenuItems();
    windowLayout = new QVBoxLayout();
    this->setLayout(windowLayout);


    changeExpreriment(Experiment::NO_EXPERIMENT);


};

void MainWindow::initMenuItems() {

    captionLabel = new QLabel(QString::fromUtf8("MAIN MENU"));
    captionLabel->setAlignment(Qt::AlignHCenter);

    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        menuButtons.push_back(new QPushButton(experiments[i]));
        menuButtons[i]->setStyleSheet("Text-align:left; padding: 3px 15px;");

        QObject::connect(menuButtons[i], SIGNAL(released()), this, SLOT(menuButtonClick()));
    }

}

void MainWindow::showMenu() {

    windowLayout->addWidget(this->captionLabel);
    windowLayout->setContentsMargins(MAIN_WIDTH / 6, -1, MAIN_WIDTH / 6, -1);


    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        windowLayout->addWidget(menuButtons[i]);

    }
    windowLayout->addStretch();

}

void MainWindow::hideMenu() {
    windowLayout->removeWidget(this->captionLabel);
    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        windowLayout->removeWidget(menuButtons[i]);
    }
    windowLayout->setContentsMargins(-1, -1, -1, -1);

}


void MainWindow::menuButtonClick() {
    QPushButton *sender = (QPushButton *) QObject::sender();
    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        if (sender == menuButtons[i])
            this->setWindowTitle(QString::number(i));

    }


}

void MainWindow::changeExpreriment(const int experiment) {
    if (Experiment::NO_EXPERIMENT) {
        showMenu();
    } else {

    }

}
