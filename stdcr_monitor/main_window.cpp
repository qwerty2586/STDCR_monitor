#include "main_window.h"
#include <stdcr_monitor/params.h>
#include <stdcr_monitor/experiments/ERP.h>
#include <stdcr_monitor/experiments/BCI.h>
#include <stdcr_monitor/experiments/RESPONSE_EXPERIMENT.h>
#include <stdcr_monitor/experiments/AUTOSTIMULATION.h>
#include <stdcr_monitor/experiments/BIOSENSOR_LOGGER.h>
#include <stdcr_monitor/experiments/TEST_MODE.h>
#include <stdcr_comm/serials.h>
#include <c++/iostream>


const QString TEXT_CONNECT = "CONNECT";
const QString TEXT_DISCONNECT = "DISCONNECT";

const QString TEXT_START = "START";
const QString TEXT_STOP = "STOP";

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    resize(MAIN_WIDTH, MAIN_HEIGHT);

    windowLayout = NULL;
    portConnected = false;
    experimentRunning = false;
    port = new Stimulator();
    connect(port, SIGNAL(connected(bool)), this, SLOT(onPortConnected(bool)));

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
    portConnectDisconnect->setStyleSheet("color: green");
    QObject::connect(portConnectDisconnect, SIGNAL(released()), this, SLOT(portConnectDisconnectClick()));


    portLayout->addWidget(portConnectDisconnect);
    portGroup->setLayout(portLayout);

    backButton = new QPushButton("BACK TO MENU");
    QObject::connect(backButton, SIGNAL(released()), this, SLOT(backClick()));
    startStopButton = new QPushButton(TEXT_START);
    startStopButton->setEnabled(portConnected);
    QObject::connect(startStopButton, SIGNAL(released()), this, SLOT(startStopClick()));
    experimentButtonsLayout = new QHBoxLayout();
    experimentButtonsLayout->addWidget(backButton);
    experimentButtonsLayout->addStretch(2);
    experimentButtonsLayout->addWidget(startStopButton);
    experimentButtonsLayout->addStretch(2);
    experimentButtonsWidget = new QWidget();
    experimentButtonsWidget->setLayout(experimentButtonsLayout);


    connect(port, SIGNAL(incomingMessage(char, QByteArray)), this, SLOT(debugpacket(char, QByteArray)));



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
    for (int i = 0; i < experiments.size(); i++) {
        experiments[i]->setStimulator(port);
        connect(experiments[i], SIGNAL(experimentStateChanged(bool)), this, SLOT(onExperimentStateChanged(bool)));
    }
}


void MainWindow::showMenu() {

    layout()->setContentsMargins(MAIN_WIDTH / 6, -1, MAIN_WIDTH / 6, -1); // pekne okraje kolem tlacitek
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
    if (!experimentRunning)
        experiments[activeExperiment]->changeExperimentState(true);
    else
        experiments[runningExperimentIndex]->changeExperimentState(false);


}

void MainWindow::portConnectDisconnectClick() {
    if (!portConnected) { //CONNECT
        port->setFile(portCombo->currentText());
    } else {            //disconnect
        port->portDisconnect();
    }

}

void MainWindow::onPortConnected(bool connected) {
    portConnected = connected;
    portCombo->setEnabled(!connected);
    startStopButton->setEnabled(connected);
    if (connected) {
        portConnectDisconnect->setText(TEXT_DISCONNECT);
        portConnectDisconnect->setStyleSheet("color: red");

    } else {
        portConnectDisconnect->setText(TEXT_CONNECT);
        portConnectDisconnect->setStyleSheet("color: green");
    }
}

void MainWindow::onExperimentStateChanged(bool state) {
    for (int i = 0; i < experiments.size(); ++i) {
        if (sender() == experiments[i]) runningExperimentIndex = i;
    }
    experimentRunning = state;
    if (state) {
        startStopButton->setText(TEXT_STOP);
    } else {
        startStopButton->setText(TEXT_START);
    }

}

void MainWindow::debugpacket(char c, QByteArray a) {
    std::cout << c << ":  ";
    std::cout << a.data();
    std::cout << std::endl;

}

MainWindow::~MainWindow() {
    port->portDisconnect();
    delete port;
}













