#include "RESPONSE_EXPERIMENT.h"
#include <QVBoxLayout>
#include <QLabel>
#include <stdcr_monitor/widgets/qsaveloadwidget.h>
#include <QFile>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include "../params.h"

RESPONSE_EXPERIMENT::RESPONSE_EXPERIMENT(QWidget *parent) : Experiment(parent) {
    initItems();

};


void RESPONSE_EXPERIMENT::initItems() {
    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    tabs->addTab(new QWidget(), "TEST SETUP");
    tabs->addTab(new QWidget(), "TEST MODE");
    tabs->addTab(new QWidget(), "SCHEMA");
    l->addWidget(tabs);
    this->setLayout(l);


    // TEST SETUP
    QGridLayout *testSetupLayout = new QGridLayout();
    tabs->widget(0)->setLayout(testSetupLayout);

    testSetupLayout->setColumnStretch(0, 2);
    testSetupLayout->setColumnStretch(1, 1);
    testSetupLayout->setColumnStretch(2, 1);

    testSetupLayout->addWidget(new QLabel("LED Count"), 0, 0);
    led_count = new QSpinBox();
    led_count->setRange(1, MAX_LEDS);
    testSetupLayout->addWidget(led_count, 0, 1);

    testSetupLayout->addWidget(new QLabel("Cycles"), 1, 0);
    cycles = new QSpinBox();
    testSetupLayout->addWidget(cycles, 1, 1);

    testSetupLayout->addWidget(new QLabel("Wait time - fixed"), 2, 0);
    wait_time_fixed = new QSpinBox();
    wait_time_fixed->setRange(MIN_MS, MAX_MS);
    testSetupLayout->addWidget(wait_time_fixed, 2, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_MS), 2, 2);

    testSetupLayout->addWidget(new QLabel("Wait time - random"), 3, 0);
    wait_time_random = new QSpinBox();
    wait_time_random->setRange(MIN_MS, MAX_MS);
    testSetupLayout->addWidget(wait_time_random, 3, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_MS), 3, 2);

    testSetupLayout->addWidget(new QLabel("Miss time"), 4, 0);
    miss_time = new QSpinBox();
    miss_time->setRange(MIN_MS, MAX_MS);
    testSetupLayout->addWidget(miss_time, 4, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_MS), 4, 2);

    testSetupLayout->addWidget(new QLabel("On fail"), 5, 0);
    cont = new QRadioButton("Continue");
    cont->setChecked(true);
    testSetupLayout->addWidget(cont, 5, 1);

    wait = new QRadioButton("Wait");
    testSetupLayout->addWidget(wait, 6, 1);

    testSetupLayout->addWidget(new QLabel("Brightness"), 7, 0);
    brightness = new QSpinBox();
    brightness->setRange(MIN_P, MAX_P);
    testSetupLayout->addWidget(brightness, 7, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_PERCENT), 7, 2);

    m = new QCheckBox("M");
    testSetupLayout->addWidget(m, 8, 0);

    f = new QCheckBox("F");
    testSetupLayout->addWidget(f, 8, 1);

    a = new QCheckBox("A");
    testSetupLayout->addWidget(a, 9, 0);

    w = new QCheckBox("W");
    testSetupLayout->addWidget(w, 9, 1);

    h = new QCheckBox("H");
    testSetupLayout->addWidget(h, 9, 2);

    //TEST MODE
    QGridLayout *test_mode_layout = new QGridLayout();

    tabs->widget(1)->setLayout(test_mode_layout);

    test_mode_layout->setColumnStretch(0, 2);
    test_mode_layout->setColumnStretch(1, 1);
    test_mode_layout->setColumnStretch(2, 1);

    test_mode_layout->addWidget(new QLabel("Count cycle"), 0, 0);
    count_cycle= new QLineEdit();
    count_cycle->setReadOnly(true);
    test_mode_layout->addWidget(count_cycle, 0, 1);

    test_mode_layout->addWidget(new QLabel("Last time"), 1, 0);
    last_time = new QLineEdit();
    last_time->setReadOnly(true);
    test_mode_layout->addWidget(last_time, 1, 1);
    test_mode_layout->addWidget(new QLabel(TEXT_MS), 1, 2);

    test_mode_layout->addWidget(new QLabel("Average time"), 2, 0);
    average_time = new QLineEdit();
    average_time->setReadOnly(true);
    test_mode_layout->addWidget(average_time, 2, 1);
    test_mode_layout->addWidget(new QLabel(TEXT_MS), 2, 2);

    test_mode_layout->addWidget(new QLabel("Miss counter"), 3, 0);
    miss_counter = new QLineEdit();
    miss_counter->setReadOnly(true);
    test_mode_layout->addWidget(miss_counter, 3, 1);

    test_mode_layout->addWidget(new QLabel("Fail counter"), 4, 0);
    fail_counter = new QLineEdit();
    fail_counter->setReadOnly(true);
    test_mode_layout->addWidget(fail_counter, 4, 1);

    test_mode_layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding), 4, 0, 3, 1);

    test_mode_layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored), 4, 0, 3, 0);

    //SCHEMA
    QGridLayout *schemaLayout = new QGridLayout();
    tabs->widget(2)->setLayout(schemaLayout);
    QSaveLoadWidget *qSaveLoadWidget = new QSaveLoadWidget(SCHEMAS_DIR, "re_", SCHEMAS_EXTENSION);
    schemaLayout->addWidget(qSaveLoadWidget, 0, 0);
    connect(qSaveLoadWidget, SIGNAL(load(QString)), this, SLOT(loadFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), this, SLOT(saveFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), qSaveLoadWidget, SLOT(refreshList()));


}


const QString DATA_ROOT = "response_experiment";
const QString DATA_ROOT_TEST_SETUP = "test_setup";
const QString DATA_LED_COUNT = "led_count";
const QString DATA_CYCLES = "cycles";
const QString DATA_WAIT_TIME_FIXED = "wait_time_fixed";
const QString DATA_WAIT_TIME_RANDOM = "wait_time_random";
const QString DATA_MISS_TIME = "miss_time";
const QString DATA_ONFAIL = "onfail";
const QString DATA_ONFAIL_CONTINUE = "continue";
const QString DATA_ONFAIL_WAIT = "wait";
const QString DATA_BRIGHTNESS = "brightness";
const QString DATA_TRUE = "true";
const QString DATA_FALSE = "false";
const QString DATA_M = "m";
const QString DATA_F = "f";
const QString DATA_A = "a";
const QString DATA_W = "w";
const QString DATA_H = "h";

bool dataBool(QString s) {
    return (DATA_TRUE == s);
};

QString dataBool(bool b) {
    if (b) return DATA_TRUE;
    else return DATA_FALSE;
};

void RESPONSE_EXPERIMENT::loadFile(QString filepathname) {
    QDomDocument doc;
    QFile file(filepathname);
    QString value = "";
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomNode root = doc.elementsByTagName(DATA_ROOT).item(0);
    led_count->setValue(root.firstChildElement(DATA_LED_COUNT).text().toInt());
    cycles->setValue(root.firstChildElement(DATA_CYCLES).text().toInt());
    wait_time_fixed->setValue(root.firstChildElement(DATA_WAIT_TIME_FIXED).text().toInt());
    wait_time_random->setValue(root.firstChildElement(DATA_WAIT_TIME_RANDOM).text().toInt());
    miss_time->setValue(root.firstChildElement(DATA_MISS_TIME).text().toInt());
    value = root.firstChildElement(DATA_ONFAIL).text().toLower();
    if (value == DATA_ONFAIL_CONTINUE) cont->setChecked(true);
    if (value == DATA_ONFAIL_WAIT) wait->setChecked(true);
    brightness->setValue(root.firstChildElement(DATA_BRIGHTNESS).text().toInt());

    m->setChecked(dataBool(root.firstChildElement(DATA_M).text()));
    f->setChecked(dataBool(root.firstChildElement(DATA_F).text()));
    a->setChecked(dataBool(root.firstChildElement(DATA_A).text()));
    w->setChecked(dataBool(root.firstChildElement(DATA_W).text()));
    h->setChecked(dataBool(root.firstChildElement(DATA_H).text()));

    file.close();
}


void RESPONSE_EXPERIMENT::saveFile(QString filepathname) {
    QFile file(filepathname);
    QString value = "";
    if (file.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement(DATA_ROOT);
        xml.writeTextElement(DATA_LED_COUNT, led_count->text());
        xml.writeTextElement(DATA_CYCLES, cycles->text());
        xml.writeTextElement(DATA_WAIT_TIME_FIXED, wait_time_fixed->text());
        xml.writeTextElement(DATA_WAIT_TIME_RANDOM, wait_time_random->text());
        xml.writeTextElement(DATA_MISS_TIME, miss_time->text());
        if (cont->isChecked()) value = DATA_ONFAIL_CONTINUE;
        if (wait->isChecked()) value = DATA_ONFAIL_WAIT;
        xml.writeTextElement(DATA_ONFAIL, value);
        xml.writeTextElement(DATA_BRIGHTNESS, brightness->text());

        xml.writeTextElement(DATA_M, dataBool(m->isChecked()));
        xml.writeTextElement(DATA_F, dataBool(f->isChecked()));
        xml.writeTextElement(DATA_A, dataBool(a->isChecked()));
        xml.writeTextElement(DATA_W, dataBool(w->isChecked()));
        xml.writeTextElement(DATA_H, dataBool(h->isChecked()));

        xml.writeEndElement();
        xml.writeEndDocument();

        file.close();


    }
}





