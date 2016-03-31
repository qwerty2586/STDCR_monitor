
#include "ERP.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QXmlStreamReader>
#include <QFile>
#include <QMessageBox>
#include "../widgets/qsaveloadwidget.h"


const static QString ERP_MS = "[ms]";
const static QString ERP_OUT = "OUT";
const static QString ERP_WAIT = "WAIT";
const static QString ERP_EDGE = "EDGE";
const static QString ERP_RAND = "RAND";

const static int MAX_LEDS = 8;




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

    syncSetupLayout->addWidget(new QLabel(ERP_OUT), 0, 0);
    q_out = new QSpinBox();
    q_out->setRange(0, 30000);
    syncSetupLayout->addWidget(q_out, 0, 1);
    syncSetupLayout->addWidget(new QLabel(ERP_MS), 0, 2);

    syncSetupLayout->addWidget(new QLabel(ERP_WAIT), 1, 0);
    q_wait = new QSpinBox();
    q_wait->setRange(0, 30000);
    syncSetupLayout->addWidget(q_wait, 1, 1);
    syncSetupLayout->addWidget(new QLabel(ERP_MS), 1, 2);

    syncSetupLayout->addWidget(new QLabel(ERP_EDGE), 2, 0);
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

    syncSetupLayout->addWidget(new QLabel(ERP_RAND), 3, 0);
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

    syncSetupLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding), 4, 0, 3,
                             1); //odsazeni


    // 2nd tab

    QGridLayout *ledSetupLayout = new QGridLayout();
    tabs->widget(1)->setLayout(ledSetupLayout);

    ledSetupLayout->setColumnStretch(0, 1);
    ledSetupLayout->setColumnStretch(1, 2);
    ledSetupLayout->setColumnStretch(2, 2);
    ledSetupLayout->setColumnStretch(3, 2);

    ledSetupLayout->addWidget(new QLabel("PULSE [ms]"), 0, 1);
    ledSetupLayout->addWidget(new QLabel("DISTRIBUTION [ms]"), 0, 2);
    ledSetupLayout->addWidget(new QLabel("BRIGHTNESS"), 0, 3);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("UP"));
    qhBoxLayout->addWidget(new QLabel("DOWN"));
    ledSetupLayout->addLayout(qhBoxLayout, 1, 1);

    qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("VALUE"));
    qhBoxLayout->addWidget(new QLabel("DELAY"));
    ledSetupLayout->addLayout(qhBoxLayout, 1, 2);

    ledSetupLayout->addWidget(new QLabel("[%]"), 1, 3);

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
    QSaveLoadWidget *qSaveLoadWidget = new QSaveLoadWidget("./schemas", "erp_", ".xml");
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
    leds[row]->pulse_down = new QSpinBox();
    leds[row]->dist_value = new QSpinBox();
    leds[row]->dist_delay = new QSpinBox();
    leds[row]->brightness = new QSpinBox();

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


void ERP::loadFile(QString filepathname) {
    QFile file(filepathname);
    if (file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader xml(&file);


    }
}

const QString DATA_ROOT = "ERP";
const QString DATA_ROOT_SYNC = "SYNC_SETUP";
const QString DATA_OUT = "OUT";
const QString DATA_WAIT = "WAIT";
const QString DATA_EDGE = "EDGE";
const QString DATA_EDGE_UP = "UP";
const QString DATA_EDGE_DOWN = "DOWN";
const QString DATA_RAND = "RAND";
const QString DATA_RAND_NONE = "NONE";
const QString DATA_RAND_PLUS = "PLUS";
const QString DATA_RAND_MINUS = "MINUS";
const QString DATA_RAND_PLUSMINUS = "BOTH";

const QString DATA_ROOT_LEDS = "LEDS_SETUP";
const QString DATA_ROOT_LED = "LED";
const QString DATA_PULSE_UP = "PULSE_UP";
const QString DATA_PULSE_DOWN = "PULSE_DOWN";
const QString DATA_DISTRIBUTION_VALUE = "DISTRIBUTION_VALUE";
const QString DATA_DISTRIBUTION_DELAY = "DISTRIBUTION_DELAY";
const QString DATA_BRIGHTNESS = "BRIGHTNESS";

void ERP::saveFile(QString filepathname) {
    QFile file(filepathname);
    QString value = "";
    if (file.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement(DATA_ROOT);

        xml.writeStartElement(DATA_ROOT_SYNC);
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
        xml.writeEndElement();

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

    }
}






