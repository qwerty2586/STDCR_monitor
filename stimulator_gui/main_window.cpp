#include "main_window.h"
#include <stimulator_gui/params.h>
#include <stimulator_gui/experiments/ERP.h>
#include <stimulator_gui/experiments/BCI.h>
#include <stimulator_gui/experiments/RESPONSE_EXPERIMENT.h>
#include <stimulator_gui/experiments/AUTOSTIMULATION.h>
#include <stimulator_gui/experiments/BIOSENSOR_LOGGER.h>
#include <stimulator_gui/experiments/TEST_MODE.h>
#include <stimulator_gui/experiments/STIMULATOR_OUTPUTS.h>
#include <stimulator_comm/sysname.h>
#include <iostream>
#include <QApplication>
#include <QLibraryInfo>


const QString TEXT_CONNECT = "CONNECT";
const QString TEXT_DISCONNECT = "DISCONNECT";

const QString TEXT_START = "START";
const QString TEXT_STOP = "STOP";

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    resize(MAIN_WIDTH, MAIN_HEIGHT);
    
    port = new Stimulator();
    connect(port, SIGNAL(connected(bool)), this, SLOT(onPortConnected(bool)));

    fileserver = new Fileserver(getSystemName(),FILESERVER_DIR);

    connect(port, SIGNAL(incomingFileserverMessage(QByteArray)), fileserver, SLOT(incomingMessage(QByteArray)));
    connect(fileserver, SIGNAL(outcomingMessage(QByteArray)), port, SLOT(sendFileserverMessage(QByteArray)));

    auto sdlOutputLauncher = new SdlOutputLauncher();

    connect(fileserver, SIGNAL(startSdlOutput(QString)), sdlOutputLauncher, SLOT(doStartSdlOutput(QString)));
    connect(fileserver, SIGNAL(stopSdlOutput()), sdlOutputLauncher, SLOT(doStopSdlOutput()));



    initExperiments();
    initItems();
    windowLayout = new QVBoxLayout(this);
    this->setLayout(windowLayout);

    
    
    statusBar = new QStatusBar(this);
    statusBar->addWidget(new QLabel(""));
    statusBar->setSizeGripEnabled(false);
    statusBar->show();
    statusBar->setMinimumWidth(this->width());
    statusBar->setMaximumWidth(this->width());
    statusBar->move(0,this->height()-statusBar->height());


    changeExperiment(Experiment::NO_EXPERIMENT);

    //setStyleSheet("border: 1px solid red"); // debugging

};

void MainWindow::initItems() {
    // main menu

    captionLabel = new QLabel(QString::fromUtf8("MAIN MENU"));
    captionLabel->setAlignment(Qt::AlignHCenter);

    for (int i = 0; i < experiments.size(); i++) {
        menuButtons.push_back(new QPushButton(experiments[i]->getName()));
        menuButtons[i]->setStyleSheet("Text-align:left; padding: 3px 15px;");
        menuButtons[i]->setEnabled(experiments[i]->isImplemented());

        connect(menuButtons[i], SIGNAL(released()), this, SLOT(menuButtonClick()));
        connect(menuButtons[i],SIGNAL(released()),experiments[i],SLOT(enterExperiment()));
    }

    // experiment related
    experimentNameLabel = new QLabel();
    experimentNameLabel->setAlignment(Qt::AlignHCenter);

    portGroup = new QGroupBox("PORT");
    portLayout = new QHBoxLayout();
    portCombo = new QComboBox();
    connect(portCombo, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(refreshPortList()));
    portLayout->addWidget(portCombo);
    portBaudCombo = new QComboBox();
    portLayout->addWidget(portBaudCombo);
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
    experiments.push_back(new STIMULATOR_OUTPUTS);
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
    layout()->addItem(new QSpacerItem(0,statusBar->height()));
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
    layout()->addItem(new QSpacerItem(0,statusBar->height()));
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
    experiments[activeExperiment]->leaveExperiment();
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
        port->setPort(port_infos[portCombo->currentIndex()],port_baudrates[portBaudCombo->currentIndex()]);
    } else {            //disconnect
        port->portDisconnect();
    }

}

void MainWindow::onPortConnected(bool connected) {
    portConnected = connected;
    portCombo->setEnabled(!connected);
    portBaudCombo->setEnabled(!connected);
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
        sdlOutputLauncher.doStartSdlOutput("");
    } else {
        startStopButton->setText(TEXT_START);
        sdlOutputLauncher.doStopSdlOutput();
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

    {
        QString last = "";
        if (portCombo->currentIndex() >= 0) {
            last = port_infos[portCombo->currentIndex()].portName();
        }
        port_infos = port->getPortsList();
        portCombo->blockSignals(true);
        portCombo->clear();
        for (const QSerialPortInfo &info : port_infos) {
            portCombo->addItem(info.portName() + " " + info.description() + " " + info.manufacturer());
            if (last.compare(info.portName()) == 0) portCombo->setCurrentIndex(portCombo->count() - 1);
        }
        portCombo->blockSignals(false);
    }{
        qint32 last = 9600;
        if (portBaudCombo->currentIndex() >= 0) {
            last = port_baudrates[portBaudCombo->currentIndex()];
        }
        port_baudrates = port->getBaudrates();
        portBaudCombo->blockSignals(true);
        portBaudCombo->clear();
        for (qint32 speed : port_baudrates) {
            portBaudCombo->addItem(QString::number(speed));
            if (last == speed) portBaudCombo->setCurrentIndex(portBaudCombo->count() - 1);
        }
        portBaudCombo->blockSignals(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent *resizeEvent) {
    QWidget::resizeEvent(resizeEvent);
    statusBar->setMinimumWidth(this->width());
    statusBar->setMaximumWidth(this->width());
    statusBar->move(0,this->height()-statusBar->height());
}















