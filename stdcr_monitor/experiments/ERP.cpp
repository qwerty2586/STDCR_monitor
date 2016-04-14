
#include "ERP.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include <QtXml/QDomDocument>
#include <stdcr_monitor/widgets/qsaveloadwidget.h>
#include <stdcr_monitor/params.h>
#include <stdcr_comm/stimulator.h>


ERP::ERP(QWidget *parent) : Experiment(parent) {
    initItems();

};



void ERP::initItems() {
    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    tabs->addTab(new QWidget(), "SYNC SETUP");
    tabs->addTab(new QWidget(), "LED SETUP");
    tabs->addTab(new QWidget(), "SCHEMA");
    l->addWidget(tabs);
    this->setLayout(l);


    // 1st tab
    QGridLayout *syncSetupLayout = new QGridLayout();
    tabs->widget(0)->setLayout(syncSetupLayout);

    syncSetupLayout->setColumnStretch(0, 2);
    syncSetupLayout->setColumnStretch(1, 1);
    syncSetupLayout->setColumnStretch(2, 1);

    syncSetupLayout->addWidget(new QLabel("OUT"), 0, 0);
    q_out = new QSpinBox();
    q_out->setRange(MIN_MS, MAX_MS);
    syncSetupLayout->addWidget(q_out, 0, 1);
    syncSetupLayout->addWidget(new QLabel(TEXT_MS), 0, 2);

    syncSetupLayout->addWidget(new QLabel("WAIT"), 1, 0);
    q_wait = new QSpinBox();
    q_wait->setRange(MIN_MS, MAX_MS);
    syncSetupLayout->addWidget(q_wait, 1, 1);
    syncSetupLayout->addWidget(new QLabel(TEXT_MS), 1, 2);

    syncSetupLayout->addWidget(new QLabel("EDGE"), 2, 0);
    QGroupBox *qGroubBox = new QGroupBox();
    qGroubBox->setFlat(true); // udelame maly groupbox
    qGroubBox->setStyleSheet("margin:0;padding:0;border:0;");
    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    q_edge_up = new QRadioButton("UP");
    q_edge_down = new QRadioButton("DOWN");
    q_edge_up->setChecked(true);
    qhBoxLayout->addWidget(q_edge_up);
    qhBoxLayout->addWidget(q_edge_down);
    qGroubBox->setLayout(qhBoxLayout);
    syncSetupLayout->addWidget(qGroubBox, 2, 1, Qt::AlignLeft);

    syncSetupLayout->addWidget(new QLabel("RAND"), 3, 0);
    qGroubBox = new QGroupBox();
    qGroubBox->setFlat(true); // udelame maly groupbox
    qGroubBox->setStyleSheet("margin:0;padding:0;border:0;");
    qhBoxLayout = new QHBoxLayout();
    q_rand_none = new QRadioButton("0");
    q_rand_plus = new QRadioButton("+");
    q_rand_minus = new QRadioButton("-");
    q_rand_plusminus = new QRadioButton("+ -");
    q_rand_none->setChecked(true);
    qhBoxLayout->addWidget(q_rand_none);
    qhBoxLayout->addWidget(q_rand_plus);
    qhBoxLayout->addWidget(q_rand_minus);
    qhBoxLayout->addWidget(q_rand_plusminus);
    qGroubBox->setLayout(qhBoxLayout);
    syncSetupLayout->addWidget(qGroubBox, 3, 1, Qt::AlignLeft);

    syncSetupLayout->addItem(
            new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding), 4, 0, 3, 1); //odsazeni


    // 2nd tab

    QGridLayout *ledSetupLayout = new QGridLayout();
    tabs->widget(1)->setLayout(ledSetupLayout);

    ledSetupLayout->setColumnStretch(0, 1);
    ledSetupLayout->setColumnStretch(1, 2);
    ledSetupLayout->setColumnStretch(2, 2);
    ledSetupLayout->setColumnStretch(3, 2);

    ledSetupLayout->addWidget(new QLabel("PULSE " + TEXT_MS), 0, 1);
    ledSetupLayout->addWidget(new QLabel("DISTRIBUTION [\%/COUNT]"), 0, 2);
    ledSetupLayout->addWidget(new QLabel("BRIGHTNESS " + TEXT_PERCENT), 0, 3);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("UP"));
    qhBoxLayout->addWidget(new QLabel("DOWN"));
    ledSetupLayout->addLayout(qhBoxLayout, 1, 1);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("VALUE"));
    qhBoxLayout->addWidget(new QLabel("DELAY"));
    ledSetupLayout->addLayout(qhBoxLayout, 1, 2);

    ledSetupLayout->addWidget(new QLabel(TEXT_PERCENT), 1, 3);

    q_add_led = new QPushButton("+");
    ledSetupLayout->addWidget(q_add_led, 2, 0);
    connect(q_add_led, SIGNAL(released()), this, SLOT(addLed()));

    q_clear_leds = new QPushButton("CLEAR");
    ledSetupLayout->addWidget(q_clear_leds, 0, 0);
    connect(q_clear_leds, SIGNAL(released()), this, SLOT(clearLeds()));
    connect(q_clear_leds, SIGNAL(released()), this, SLOT(addLed())); //a pridame prvni led
    q_clear_leds->hide();


    addLed(); // prepare first led

    ledSetupLayout->setRowMinimumHeight(0, 30);
    ledSetupLayout->setRowStretch(MAX_LEDS + 4, 1); //nejake to roztazeni


    // schema tab
    QGridLayout *schemeLayout = new QGridLayout();
    tabs->widget(2)->setLayout(schemeLayout);
    QSaveLoadWidget *qSaveLoadWidget = new QSaveLoadWidget(SCHEMAS_DIR, "erp_", SCHEMAS_EXTENSION);
    schemeLayout->addWidget(qSaveLoadWidget, 0, 0);
    connect(qSaveLoadWidget, SIGNAL(load(QString)), this, SLOT(loadFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), this, SLOT(saveFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), qSaveLoadWidget, SLOT(refreshList()));

}

void ERP::addLed() {
    int row = leds.size();

    QGridLayout *ledSetupLayout = (QGridLayout *) tabs->widget(1)->layout();

    q_add_led->hide();

    ledSetupLayout->removeWidget(q_add_led);
    ledSetupLayout->addWidget(new QLabel(QString::fromUtf8("LED") + QString::number(row)), row + 2, 0);

    leds.push_back(new Led());
    leds[row]->pulse_up = new QSpinBox();
    leds[row]->pulse_up->setRange(MIN_MS, MAX_MS);

    leds[row]->pulse_down = new QSpinBox();
    leds[row]->pulse_down->setRange(MIN_MS, MAX_MS);

    leds[row]->dist_value = new QSpinBox();
    leds[row]->dist_value->setRange(MIN_P, MAX_P);

    leds[row]->dist_delay = new QSpinBox();
    leds[row]->dist_delay->setRange(MIN_COUNT, MAX_COUNT);

    leds[row]->brightness = new QSpinBox();
    leds[row]->brightness->setRange(MIN_P, MAX_P);

    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(leds[row]->pulse_up);
    qhBoxLayout->addWidget(leds[row]->pulse_down);
    ledSetupLayout->addLayout(qhBoxLayout, row + 2, 1);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(leds[row]->dist_value);
    qhBoxLayout->addWidget(leds[row]->dist_delay);
    ledSetupLayout->addLayout(qhBoxLayout, row + 2, 2);

    ledSetupLayout->addWidget(leds[row]->brightness, row + 2, 3);

    // add + button back
    if (row < MAX_LEDS - 1) {
        ledSetupLayout->addWidget(q_add_led, row + 3, 0);
        q_add_led->show();
    }

    // show clear button
    if (row > 0) {
        q_clear_leds->show();
    }
}


void ERP::clearLeds() {
    QGridLayout *ledSetupLayout = (QGridLayout *) tabs->widget(1)->layout();
    ledSetupLayout->removeWidget(q_add_led);


    for (int i = 0; i < leds.size(); i++) {

        delete ledSetupLayout->itemAtPosition(i + 2, 0)->widget();
        delete ledSetupLayout->itemAtPosition(i + 2, 1)->layout();
        delete ledSetupLayout->itemAtPosition(i + 2, 2)->layout();

        delete leds[i]->pulse_up;
        delete leds[i]->pulse_down;
        delete leds[i]->dist_value;
        delete leds[i]->dist_delay;

        delete leds[i]->brightness;

    }
    leds.clear();
    ledSetupLayout->addWidget(q_add_led, 2, 0);


    q_clear_leds->hide(); // hide button for clearing leds

}


const QString DATA_ROOT = "erp";
const QString DATA_OUT = "out";
const QString DATA_WAIT = "wait";
const QString DATA_EDGE = "edge";
const QString DATA_EDGE_UP = "up";
const QString DATA_EDGE_DOWN = "down";
const QString DATA_RAND = "rand";
const QString DATA_RAND_NONE = "none";
const QString DATA_RAND_PLUS = "plus";
const QString DATA_RAND_MINUS = "minus";
const QString DATA_RAND_PLUSMINUS = "both";

const QString DATA_ROOT_LEDS = "leds_setup";
const QString DATA_ROOT_LED = "led";
const QString DATA_PULSE_UP = "pulse_up";
const QString DATA_PULSE_DOWN = "pulse_down";
const QString DATA_DISTRIBUTION_VALUE = "distribution_value";
const QString DATA_DISTRIBUTION_DELAY = "distribution_delay";
const QString DATA_BRIGHTNESS = "brightness";

void ERP::loadFile(QString filepathname) {
    QDomDocument doc;
    QFile file(filepathname);
    QString value = "";
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomNode rootnode = doc.elementsByTagName(DATA_ROOT).item(0);
    q_out->setValue(rootnode.firstChildElement(DATA_OUT).text().toInt());
    q_wait->setValue(rootnode.firstChildElement(DATA_WAIT).text().toInt());
    value = rootnode.firstChildElement(DATA_EDGE).text().toLower();
    if (value == DATA_EDGE_UP) q_edge_up->setChecked(true);
    if (value == DATA_EDGE_DOWN) q_edge_down->setChecked(true);
    value = rootnode.firstChildElement(DATA_RAND).text().toLower();
    if (value == DATA_RAND_NONE) q_rand_none->setChecked(true);
    if (value == DATA_RAND_PLUS) q_rand_plus->setChecked(true);
    if (value == DATA_RAND_MINUS) q_rand_minus->setChecked(true);
    if (value == DATA_RAND_PLUSMINUS) q_rand_plusminus->setChecked(true);

    QDomNodeList ledlist = doc.elementsByTagName(DATA_ROOT_LED);
    clearLeds();
    for (int i = 0; i < ledlist.size(); i++) {
        addLed();
        QDomNode lednode = ledlist.item(i);
        leds[i]->pulse_up->setValue(lednode.firstChildElement(DATA_PULSE_UP).text().toInt());
        leds[i]->pulse_down->setValue(lednode.firstChildElement(DATA_PULSE_DOWN).text().toInt());
        leds[i]->dist_value->setValue(lednode.firstChildElement(DATA_DISTRIBUTION_VALUE).text().toInt());
        leds[i]->dist_delay->setValue(lednode.firstChildElement(DATA_DISTRIBUTION_DELAY).text().toInt());
        leds[i]->brightness->setValue(lednode.firstChildElement(DATA_BRIGHTNESS).text().toInt());
    }

}

void ERP::saveFile(QString filepathname) {
    //musime zobrazit vsechny prvky jinak v nich nebudou hodnoty... just QT thing
    int backup = tabs->currentIndex();
    tabs->setCurrentIndex(1);
    tabs->setCurrentIndex(2);
    tabs->setCurrentIndex(backup);

    QFile file(filepathname);
    QString value = "";
    if (file.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement(DATA_ROOT);

        xml.writeTextElement(DATA_OUT, q_out->text());
        xml.writeTextElement(DATA_WAIT, q_wait->text());
        if (q_edge_up->isChecked()) value = DATA_EDGE_UP;
        if (q_edge_down->isChecked()) value = DATA_EDGE_DOWN;
        xml.writeTextElement(DATA_EDGE, value);
        if (q_rand_none->isChecked()) value = DATA_RAND_NONE;
        if (q_rand_plus->isChecked()) value = DATA_RAND_PLUS;
        if (q_rand_minus->isChecked()) value = DATA_RAND_MINUS;
        if (q_rand_plusminus->isChecked()) value = DATA_RAND_PLUSMINUS;
        xml.writeTextElement(DATA_RAND, value);

        xml.writeStartElement(DATA_ROOT_LEDS);
        for (int i = 0; i < leds.size(); ++i) {
            xml.writeStartElement(DATA_ROOT_LED);
            xml.writeTextElement(DATA_PULSE_UP, leds[i]->pulse_up->text());
            xml.writeTextElement(DATA_PULSE_DOWN, leds[i]->pulse_down->text());
            xml.writeTextElement(DATA_DISTRIBUTION_VALUE, leds[i]->dist_value->text());
            xml.writeTextElement(DATA_DISTRIBUTION_DELAY, leds[i]->dist_delay->text());
            xml.writeTextElement(DATA_BRIGHTNESS, leds[i]->brightness->text());
            xml.writeEndElement();
        }
        xml.writeEndElement();

        xml.writeEndElement();
        xml.writeEndDocument();

        file.close();
    }
}

void ERP::changeExperimentState(bool state) {
    if (state) { //START

        if (q_rand_none->isChecked())
            port->sendMessage(StimulatorMessage::RANDOM_DISABLE);
        else
            port->sendMessage(StimulatorMessage::RANDOM_ENABLE);
        // zatim nelze protokolem nastavit + - +-

        char edge = q_edge_up->isChecked() ? StimulatorMessage::PULSE_EDGE_UP : StimulatorMessage::PULSE_EDGE_DOWN;

        port->sendMessage(StimulatorMessage::PULSE_EDGE, edge);
        port->sendMessage(StimulatorMessage::SYNC_PULSE_INTERVAL, q_out->value());

        for (int i = 0; i < leds.size(); ++i) {
            port->sendMessage(StimulatorMessage::TIME_ON_LED[i],
                              leds[i]->pulse_up->value());
            port->sendMessage(StimulatorMessage::TIME_PAUSE_LED[i],
                              leds[i]->pulse_down->value());
            port->sendMessage(StimulatorMessage::DISTRIBUTION_LED[i],
                              (char) (leds[i]->dist_value->value()));
            port->sendMessage(StimulatorMessage::BRIGHTNESS_LED[i],
                              (char) (leds[i]->brightness->value()));


        }

        port->sendMessage(StimulatorMessage::LED_ENABLE);
        emit experimentStateChanged(true);
    } else { //STOP
        port->sendMessage(StimulatorMessage::LED_DISABLE);
        emit experimentStateChanged(false);
    }
}











