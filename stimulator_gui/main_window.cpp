#include "main_window.h"
#include <stimulator_gui/params.h>
#include <stimulator_gui/experiments/ERP.h>
#include <stimulator_gui/experiments/BCI.h>
#include <stimulator_gui/experiments/RESPONSE_EXPERIMENT.h>
#include <stimulator_gui/experiments/AUTOSTIMULATION.h>
#include <stimulator_gui/experiments/BIOSENSOR_LOGGER.h>
#include <stimulator_gui/experiments/TEST_MODE.h>
#include <stimulator_comm/serials.h>
#include <stimulator_comm/sysname.h>
#include <iostream>
#include <QApplication>


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

    fileserver = new Fileserver(FILESERVER_DIR,getSystemName());

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
    connect(portCombo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(refreshPortList()));
    portLayout->addWidget(portCombo);
    portRefreshButton = new QPushButton();
    portRefreshButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    portLayout->addWidget(portRefreshButton);
    connect(portRefreshButton, SIGNAL(released()), this, SLOT(refreshPortList()));
    refreshPortList(); // pridame polozky
    portConnectDisconnectButton = new QPushButton(TEXT_CONNECT);
    portConnectDisconnectButton->setStyleSheet("color: green");
    QObject::connect(portConnectDisconnectButton, SIGNAL(released()), this, SLOT(portConnectDisconnectClick()));
    portLayout->addWidget(portConnectDisconnectButton);
    portLayout->setStretch(0, 2);
    portLayout->setStretch(1, 0);
    portLayout->setStretch(2, 1);
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
    experiments.push_back(new ERP());
    experiments.push_back(new BCI());
    experiments.push_back(new RESPONSE_EXPERIMENT());
    experiments.push_back(new AUTOSTIMULATION());
    experiments.push_back(new BIOSENSOR_LOGGER());
    experiments.push_back(new TEST_MODE());
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
    portRefreshButton->setEnabled(!connected);
    startStopButton->setEnabled(connected);
    if (connected) {
        portConnectDisconnectButton->setText(TEXT_DISCONNECT);
        portConnectDisconnectButton->setStyleSheet("color: red");

    } else {
        portConnectDisconnectButton->setText(TEXT_CONNECT);
        portConnectDisconnectButton->setStyleSheet("color: green");
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
    std::cout << QByteArray::fromRawData(&c, 1).toHex().data() << ":  ";
    std::cout << a.toHex().data();
    std::cout << std::endl;

}

MainWindow::~MainWindow() {
    port->portDisconnect();
    delete port;
}

void MainWindow::refreshPortList() {
    portCombo->clear();
    portCombo->addItems(listOfAvailableSerials());
}















