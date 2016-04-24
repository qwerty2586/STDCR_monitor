#include "RESPONSE_EXPERIMENT.h"
#include <QVBoxLayout>
#include <QLabel>
#include <stdcr_monitor/widgets/qsaveloadwidget.h>
#include <QFile>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QGroupBox>
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

    testSetupLayout->addWidget(new QLabel("LED COUNT"), 0, 0);
    led_count = new QSpinBox();
    led_count->setRange(1, MAX_LEDS);
    testSetupLayout->addWidget(led_count, 0, 1);

    testSetupLayout->addWidget(new QLabel("CYCLES"), 1, 0);
    cycles = new QSpinBox();
    testSetupLayout->addWidget(cycles, 1, 1);

    testSetupLayout->addWidget(new QLabel("WAIT TIME - FIXED"), 2, 0);
    wait_time_fixed = new QSpinBox();
    wait_time_fixed->setRange(MIN_MS, MAX_MS);
    testSetupLayout->addWidget(wait_time_fixed, 2, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_MS), 2, 2);

    testSetupLayout->addWidget(new QLabel("WAIT TIME - RANDOM"), 3, 0);
    wait_time_random = new QSpinBox();
    wait_time_random->setRange(MIN_MS, MAX_MS);
    testSetupLayout->addWidget(wait_time_random, 3, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_MS), 3, 2);

    testSetupLayout->addWidget(new QLabel("MISS TIME"), 4, 0);
    miss_time = new QSpinBox();
    miss_time->setRange(MIN_MS, MAX_MS);
    testSetupLayout->addWidget(miss_time, 4, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_MS), 4, 2);

    testSetupLayout->addWidget(new QLabel("ON FAIL"), 5, 0);
    QVBoxLayout *qvBoxLayout = new QVBoxLayout();
    QGroupBox *qGroubBox = new QGroupBox();
    qGroubBox->setFlat(true); // udelame maly groupbox
    qGroubBox->setStyleSheet("margin:0;padding:0;border:0;");
    cont = new QRadioButton("CONTINUE");
    cont->setChecked(true);
    qvBoxLayout->addWidget(cont);
    wait = new QRadioButton("WAIT");
    qvBoxLayout->addWidget(wait);
    qGroubBox->setLayout(qvBoxLayout);
    testSetupLayout->addWidget(qGroubBox, 5, 1);


    testSetupLayout->addWidget(new QLabel("BRIGHTNESS"), 6, 0);
    brightness = new QSpinBox();
    brightness->setRange(MIN_P, MAX_P);
    testSetupLayout->addWidget(brightness, 6, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_PERCENT), 6, 2);

    testSetupLayout->addWidget(new QLabel("SEX"), 7, 0);
    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    qGroubBox = new QGroupBox();
    qGroubBox->setFlat(true); // udelame maly groupbox
    qGroubBox->setStyleSheet("margin:0;padding:0;border:0;");
    male = new QRadioButton("MALE");
    male->setChecked(true);
    qhBoxLayout->addWidget(male);
    female = new QRadioButton("FEMALE");
    qhBoxLayout->addWidget(female);
    qGroubBox->setLayout(qhBoxLayout);
    testSetupLayout->addWidget(qGroubBox, 7, 1);

    testSetupLayout->addWidget(new QLabel("AGE"), 8, 0);
    age = new QSpinBox();
    age->setRange(MIN_AGE, MAX_AGE);
    testSetupLayout->addWidget(age, 8, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_YEARS), 8, 2);

    testSetupLayout->addWidget(new QLabel("WEIGHT"), 9, 0);
    body_weight = new QSpinBox();
    body_weight->setRange(MIN_BODY_WEIGHT, MAX_BODY_WEIGHT);
    testSetupLayout->addWidget(body_weight, 9, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_KG), 9, 2);

    testSetupLayout->addWidget(new QLabel("HEIGHT"), 10, 0);
    body_height = new QSpinBox();
    body_height->setRange(MIN_BODY_HEIGHT, MAX_BODY_HEIGHT);
    testSetupLayout->addWidget(body_height, 10, 1);
    testSetupLayout->addWidget(new QLabel(TEXT_CM), 10, 2);


    //TEST MODE
    QGridLayout *test_mode_layout = new QGridLayout();

    tabs->widget(1)->setLayout(test_mode_layout);

    test_mode_layout->setColumnStretch(0, 2);
    test_mode_layout->setColumnStretch(1, 1);
    test_mode_layout->setColumnStretch(2, 1);

    test_mode_layout->addWidget(new QLabel("COUNT CYCLE"), 0, 0);
    count_cycle= new QLineEdit();
    count_cycle->setReadOnly(true);
    test_mode_layout->addWidget(count_cycle, 0, 1);

    test_mode_layout->addWidget(new QLabel("LAST TIME"), 1, 0);
    last_time = new QLineEdit();
    last_time->setReadOnly(true);
    test_mode_layout->addWidget(last_time, 1, 1);
    test_mode_layout->addWidget(new QLabel(TEXT_MS), 1, 2);

    test_mode_layout->addWidget(new QLabel("AVERAGE TIME"), 2, 0);
    average_time = new QLineEdit();
    average_time->setReadOnly(true);
    test_mode_layout->addWidget(average_time, 2, 1);
    test_mode_layout->addWidget(new QLabel(TEXT_MS), 2, 2);

    test_mode_layout->addWidget(new QLabel("MISS COUNTER"), 3, 0);
    miss_counter = new QLineEdit();
    miss_counter->setReadOnly(true);
    test_mode_layout->addWidget(miss_counter, 3, 1);

    test_mode_layout->addWidget(new QLabel("FAIL COUNTER"), 4, 0);
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
const QString DATA_SEX = "sex";
const QString DATA_SEX_M = "male";
const QString DATA_SEX_F = "female";
const QString DATA_AGE = "age";
const QString DATA_WEIGHT = "weight";
const QString DATA_HEIGHT = "height";

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

    value = root.firstChildElement(DATA_SEX).text().toLower();
    if (value == DATA_SEX_M) male->setChecked(true);
    if (value == DATA_SEX_F) female->setChecked(true);
    age->setValue(root.firstChildElement(DATA_AGE).text().toInt());
    body_weight->setValue(root.firstChildElement(DATA_WEIGHT).text().toInt());
    body_height->setValue(root.firstChildElement(DATA_HEIGHT).text().toInt());
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

        if (male->isChecked()) value = DATA_SEX_M;
        if (female->isChecked()) value = DATA_SEX_F;
        xml.writeTextElement(DATA_SEX, value);

        xml.writeTextElement(DATA_AGE, age->text());
        xml.writeTextElement(DATA_WEIGHT, body_weight->text());
        xml.writeTextElement(DATA_HEIGHT, body_height->text());

        xml.writeEndElement();
        xml.writeEndDocument();

        file.close();


    }
}





