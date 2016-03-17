#include "main_window.h"
#include "params.h"
#include "experiments/ERP.h"


MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    resize(MAIN_WIDTH, MAIN_HEIGHT);

    windowLayout = NULL;
    initExperiments();
    initItems();
    windowLayout = new QVBoxLayout(this);
    this->setLayout(windowLayout);


    changeExperiment(Experiment::NO_EXPERIMENT);


};

void MainWindow::initItems() {
    // main menu

    captionLabel = new QLabel(QString::fromUtf8("MAIN MENU"));
    captionLabel->setAlignment(Qt::AlignHCenter);

    for (int i = 0; i < experiments.size(); i++) {
        menuButtons.push_back(new QPushButton(experiments[i]->getName()));
        menuButtons[i]->setStyleSheet("Text-align:left; padding: 3px 15px;");
        menuButtons[i]->setEnabled(experiments[i]->isImplemented());

        QObject::connect(menuButtons[i], SIGNAL(released()), this, SLOT(menuButtonClick()));
    }

    // experiment related
    experimentNameLabel = new QLabel();
    experimentNameLabel->setAlignment(Qt::AlignHCenter);

    backButton = new QPushButton(QString::fromUtf8("BACK TO MENU"));
    QObject::connect(backButton, SIGNAL(released()), this, SLOT(backClick()));
    startStopButton = new QPushButton(QString::fromUtf8("START"));
    QObject::connect(startStopButton, SIGNAL(released()), this, SLOT(startStopClick()));
    experimentButtonsLayout = new QHBoxLayout();
    experimentButtonsLayout->addWidget(backButton);
    experimentButtonsLayout->addStretch(2);
    experimentButtonsLayout->addWidget(startStopButton);
    experimentButtonsLayout->addStretch(2);
    experimentButtonsWidget = new QWidget();
    experimentButtonsWidget->setLayout(experimentButtonsLayout);



}

void MainWindow::initExperiments() {
    ERP *erp = new ERP();
    experiments.push_back(erp);
    experiments.push_back(erp);
    experiments.push_back(erp);
    experiments.push_back(erp);
    experiments.push_back(erp);
    experiments.push_back(erp);
}


void MainWindow::showMenu() {

    layout()->setContentsMargins(MAIN_WIDTH / 6, -1, MAIN_WIDTH / 6, -1);
    layout()->addWidget(this->captionLabel);
    captionLabel->show();
    for (int i = 0; i < experiments.size(); i++) {
        layout()->addWidget(menuButtons[i]);
        menuButtons[i]->show();

    }
    ((QVBoxLayout *) layout())->addStretch(1);
}


void MainWindow::clearLayout() {
    QLayoutItem *item;
    while ((item = layout()->takeAt(0))) {
        if (item->widget()) {
            item->widget()->hide();
            layout()->removeWidget(item->widget());
        }
        if (item->spacerItem()) {
            delete item->spacerItem();
        }
    }

}


void MainWindow::showExperiment(const int experiment) {

    layout()->setContentsMargins(-1, -1, -1, -1);
    experimentNameLabel->setText(experiments[experiment]->getName());
    layout()->addWidget(this->experimentNameLabel);
    experimentNameLabel->show();
    layout()->addWidget(experiments[experiment]);
    experiments[experiment]->show();
    ((QVBoxLayout *) layout())->addStretch(1);
    layout()->addWidget(experimentButtonsWidget);
    experimentButtonsWidget->show();



}


void MainWindow::menuButtonClick() {
    QPushButton *sender = (QPushButton *) QObject::sender();
    for (int i = 0; i < experiments.size(); i++) {
        if (sender == menuButtons[i]) {
            changeExperiment(i);
            break;
        }
    }
}


void MainWindow::changeExperiment(const int experiment) {

    if (experiment == Experiment::NO_EXPERIMENT) {
        clearLayout();
        showMenu();
    } else {
        clearLayout();
        showExperiment(experiment);
    }
    activeExperiment = experiment;

}


void MainWindow::backClick() {
    changeExperiment(Experiment::NO_EXPERIMENT);
}

void MainWindow::startStopClick() {

}

