#include "main_window.h"
#include "params.h"
#include "experiments/ERP.h"
#include "experiments/BCI.h"
#include "experiments/RESPONSE_EXPERIMENT.h"
#include "experiments/AUTOSTIMULATION.h"
#include "experiments/BIOSENSOR_LOGGER.h"
#include "experiments/TEST_MODE.h"
#include <stdcr_comm/serials.h>

const QString TEXT_CONNECT = "CONNECT";
const QString TEXT_DISCONNECT = "DISCONNECT";

const QString TEXT_START = "START";
const QString TEXT_STOP = "STOP";

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

    portGroup = new QGroupBox("PORT");
    portLayout = new QHBoxLayout();
    portCombo = new QComboBox();
    portCombo->addItems(listOfAvailableSerials());
    portLayout->addWidget(portCombo);
    portConnectDisconnect = new QPushButton(TEXT_CONNECT);

    portLayout->addWidget(portConnectDisconnect);
    portGroup->setLayout(portLayout);

    backButton = new QPushButton("BACK TO MENU");
    QObject::connect(backButton, SIGNAL(released()), this, SLOT(backClick()));
    startStopButton = new QPushButton(TEXT_START);
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
    BCI *bci = new BCI();
    experiments.push_back(bci);
    RESPONSE_EXPERIMENT *response_experiment = new RESPONSE_EXPERIMENT();
    experiments.push_back(response_experiment);
    AUTOSTIMULATION *autostimulation = new AUTOSTIMULATION();
    experiments.push_back(autostimulation);
    BIOSENSOR_LOGGER *biosensor_logger = new BIOSENSOR_LOGGER();
    experiments.push_back(biosensor_logger);
    TEST_MODE *test_mode = new TEST_MODE();
    experiments.push_back(test_mode);
}


void MainWindow::showMenu() {

    layout()->setContentsMargins(MAIN_WIDTH / 6, -1, MAIN_WIDTH / 6, -1);
    layout()->addWidget(this->captionLabel);
    captionLabel->show();
    for (int i = 0; i < experiments.size(); i++) {
        layout()->addWidget(menuButtons[i]);
        menuButtons[i]->show();

    }
    layout()->addWidget(portGroup);
    portGroup->show();
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

