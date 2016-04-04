#include <stdcr_monitor/widgets/qsaveloadwidget.h>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QXmlStreamWriter>
#include "BCI.h"

const int MAX_LEDS = 8;
const int DEFAULT_LENGTH = 8;
const int MAX_LENGTH = 32;

BCI::BCI(QWidget *parent) : Experiment(parent) {
    initItems();
};

void BCI::initItems() {

    QVBoxLayout *l = new QVBoxLayout();


    tabs = new QTabWidget();
    f_tabs = new QTabWidget();
    tabs->addTab(f_tabs, "F-VEP");
    t_tabs = new QTabWidget();
    tabs->addTab(t_tabs, "T-VEP");
    tabs->addTab(new QWidget(), "C-VEP");
    tabs->addTab(new QWidget(), "SCHEMA");
    l->addWidget(tabs);
    this->setLayout(l);

    // f-vep


    f_add_led = new QToolButton(f_tabs);
    f_add_led->setText("+");
    f_tabs->setCornerWidget(f_add_led, Qt::TopLeftCorner);
    f_clear_leds = new QToolButton(f_tabs);
    f_clear_leds->setText("CLEAR");
    f_tabs->setCornerWidget(f_clear_leds, Qt::TopRightCorner);

    connect(f_add_led, SIGNAL(released()), this, SLOT(addFLed()));
    connect(f_clear_leds, SIGNAL(released()), this, SLOT(clearFLeds()));
    connect(f_clear_leds, SIGNAL(released()), this, SLOT(addFLed()));

    addFLed();

    // t-vep

    t_add_led = new QToolButton(t_tabs);
    t_add_led->setText("+");
    t_tabs->setCornerWidget(t_add_led, Qt::TopLeftCorner);
    t_clear_leds = new QToolButton(t_tabs);
    t_clear_leds->setText("CLEAR");
    t_tabs->setCornerWidget(t_clear_leds, Qt::TopRightCorner);

    connect(t_add_led, SIGNAL(released()), this, SLOT(addTLed()));
    connect(t_clear_leds, SIGNAL(released()), this, SLOT(clearTLeds()));
    connect(t_clear_leds, SIGNAL(released()), this, SLOT(addTLed()));

    addTLed();

    // c-vep

    QGridLayout *cvepLayout = new QGridLayout();
    tabs->widget(2)->setLayout(cvepLayout);

    cvepLayout->setColumnStretch(0, 2);
    cvepLayout->setColumnStretch(1, 1);
    cvepLayout->setColumnStretch(2, 1);

    cvepLayout->addWidget(new QLabel("STIMULI COUNT"), 0, 0);
    q_stimuli_count = new QSpinBox();
    cvepLayout->addWidget(q_stimuli_count, 0, 1);

    cvepLayout->addWidget(new QLabel("PATTERN"), 1, 0);
    q_pattern = new QCheckGrid(2, 16);
    cvepLayout->addWidget(q_pattern, 2, 0, 1, 3);  // zapotrebi roztahnout prez vicero sloupku
    connect(q_pattern, SIGNAL(valueChanged()), this, SLOT(patternChanged()));

    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    qhBoxLayout->addWidget(new QLabel("TARGET BIN      ")); // potrebujeme trosku odsadit
    q_target_bin = new QLabel("");
    qhBoxLayout->addWidget(q_target_bin);
    cvepLayout->addLayout(qhBoxLayout, 3, 0, 1, 3, Qt::AlignLeft);

    cvepLayout->addWidget(new QLabel("TARGET WAVEFORM"), 4, 0);
    q_target_wave = new QWaveForm(32);
    cvepLayout->addWidget(q_target_wave, 5, 0, 1, 3);

    patternChanged(); // nastavime target bin a target wave

    cvepLayout->addWidget(new QLabel("PULSE LENGTH"), 6, 0);
    q_pulse_length = new QSpinBox();
    cvepLayout->addWidget(q_pulse_length, 6, 1);
    cvepLayout->addWidget(new QLabel("[ms]"), 6, 2);

    cvepLayout->addWidget(new QLabel("PULSE SKEW"), 7, 0);
    q_pulse_skew = new QSpinBox();
    cvepLayout->addWidget(q_pulse_skew, 7, 1);
    cvepLayout->addWidget(new QLabel("[ms]"), 7, 2);

    cvepLayout->addWidget(new QLabel("BRIGHTNESS"), 8, 0);
    q_brightness = new QSpinBox();
    q_brightness->setRange(0, 100);
    cvepLayout->addWidget(q_brightness, 8, 1);
    cvepLayout->addWidget(new QLabel("[%]"), 8, 2);

    //schema

    QGridLayout *schemaLayout = new QGridLayout();
    tabs->widget(3)->setLayout(schemaLayout);
    QSaveLoadWidget *qSaveLoadWidget = new QSaveLoadWidget("./schemas", "bci_", ".xml");
    schemaLayout->addWidget(qSaveLoadWidget, 0, 0);
    connect(qSaveLoadWidget, SIGNAL(load(QString)), this, SLOT(loadFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), this, SLOT(saveFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), qSaveLoadWidget, SLOT(refreshList()));



}


void BCI::patternChanged() {

    q_target_bin->setText(q_pattern->toNiceString()->toAscii());
    q_target_wave->setValue(q_pattern->value());

}

void BCI::addFLed() {
    int index = fleds.size();
    if (index >= MAX_LEDS)
        return;;

    f_tabs->addTab(new QWidget, QString::fromUtf8("LED") + QString::number(index));
    QGridLayout *fLedLayout = new QGridLayout();
    f_tabs->widget(index)->setLayout(fLedLayout);

    fLedLayout->setColumnStretch(0, 2);
    fLedLayout->setColumnStretch(1, 1);
    fLedLayout->setColumnStretch(2, 1);

    fleds.push_back(new FLed());

    FLed *l = fleds[index]; // zkraceni zapisu

    l->label1 = new QLabel("TIME ON");
    l->time_on = new QSpinBox();
    l->time_on->setRange(0, 30000);
    l->label1p = new QLabel("[ms]");
    fLedLayout->addWidget(l->label1, 0, 0);
    fLedLayout->addWidget(l->time_on, 0, 1);
    fLedLayout->addWidget(l->label1p, 0, 2);

    l->label2 = new QLabel("TIME OFF");
    l->time_off = new QSpinBox();
    l->time_off->setRange(0, 30000);
    l->label2p = new QLabel("[ms]");
    fLedLayout->addWidget(l->label2, 1, 0);
    fLedLayout->addWidget(l->time_off, 1, 1);
    fLedLayout->addWidget(l->label2p, 1, 2);

    l->label3 = new QLabel("FREQUENCY");
    l->frequency = new QSpinBox();
    l->frequency->setRange(0, 30000);
    l->label3p = new QLabel("[ms]");
    fLedLayout->addWidget(l->label3, 2, 0);
    fLedLayout->addWidget(l->frequency, 2, 1);
    fLedLayout->addWidget(l->label3p, 2, 2);

    l->label4 = new QLabel("DUTY CYCLE");
    l->duty_cycle = new QSpinBox();
    l->duty_cycle->setRange(0, 100);
    l->label4p = new QLabel("[%]");
    fLedLayout->addWidget(l->label4, 3, 0);
    fLedLayout->addWidget(l->duty_cycle, 3, 1);
    fLedLayout->addWidget(l->label4p, 3, 2);

    l->label5 = new QLabel("BRIGHTNESS");
    l->brightness = new QSpinBox();
    l->brightness->setRange(0, 100);
    l->label5p = new QLabel("[%]");
    fLedLayout->addWidget(l->label5, 4, 0);
    fLedLayout->addWidget(l->brightness, 4, 1);
    fLedLayout->addWidget(l->label5p, 4, 2);

}

void BCI::clearFLeds() {
    FLed *l;
    for (int i = 0; i < fleds.size(); ++i) {
        l = fleds[i];
        delete l->label1;
        delete l->label2;
        delete l->label3;
        delete l->label4;
        delete l->label5;

        delete l->label1p;
        delete l->label2p;
        delete l->label3p;
        delete l->label4p;
        delete l->label5p;

        delete l->time_on;
        delete l->time_off;
        delete l->frequency;
        delete l->duty_cycle;
        delete l->brightness;


    }
    fleds.clear();
    f_tabs->clear();
}

void BCI::addTLed() {
    int index = tleds.size();
    if (index >= MAX_LEDS)
        return;;

    t_tabs->addTab(new QWidget, QString::fromUtf8("LED") + QString::number(index));
    QGridLayout *tLedLayout = new QGridLayout();
    t_tabs->widget(index)->setLayout(tLedLayout);

    tLedLayout->setColumnStretch(0, 2);
    tLedLayout->setColumnStretch(1, 1);
    tLedLayout->setColumnStretch(2, 1);


    tleds.push_back(new TLed());

    TLed *l = tleds[index]; // zkraceni zapisu

    l->label_pattern = new QLabel("PATERN");
    tLedLayout->addWidget(l->label_pattern, 0, 0);
    l->pattern = new QCheckGrid(1, DEFAULT_LENGTH);
    tLedLayout->addWidget(l->pattern, 1, 0, 1, 3);

    l->bin_layout = new QHBoxLayout();
    l->label_bin = new QLabel("TARGET BIN      ");
    l->bin_layout->addWidget(l->label_bin);
    l->bin = new QLabel(l->pattern->toNiceString()->toAscii());
    l->bin_layout->addWidget(l->bin);
    tLedLayout->addLayout(l->bin_layout, 2, 0, 1, 3, Qt::AlignLeft);

    l->label_wave = new QLabel("TARGET WAVEFORM");
    tLedLayout->addWidget(l->label_wave, 4, 0);
    l->waveform = new QWaveForm(DEFAULT_LENGTH);
    tLedLayout->addWidget(l->waveform, 5, 0, 1, 3);
    connect(l->pattern, SIGNAL(valueChanged()), this, SLOT(tPatternChanged()));

    l->label1 = new QLabel("PATTERN LENGTH");
    tLedLayout->addWidget(l->label1, 6, 0);
    l->pattern_length = new QSpinBox();
    l->pattern_length->setRange(0, MAX_LENGTH);
    l->pattern_length->setValue(DEFAULT_LENGTH);
    tLedLayout->addWidget(l->pattern_length, 6, 1);
    connect(l->pattern_length, SIGNAL(valueChanged(int)), this, SLOT(tLengthChanged()));
    connect(l->pattern_length, SIGNAL(valueChanged(int)), l->waveform, SLOT(resizeLength(int)));

    l->label2 = new QLabel("PULSE LENGTH");
    tLedLayout->addWidget(l->label2, 7, 0);
    l->pulse_length = new QSpinBox();
    l->pulse_length->setRange(0, 30000);
    tLedLayout->addWidget(l->pulse_length, 7, 1);
    l->label2p = new QLabel("[ms]");
    tLedLayout->addWidget(l->label2p, 7, 2);

    l->label3 = new QLabel("PULSE SKEW");
    tLedLayout->addWidget(l->label3, 8, 0);
    l->pulse_skew = new QSpinBox();
    l->pulse_skew->setRange(0, 30000);
    tLedLayout->addWidget(l->pulse_skew, 8, 1);
    l->label3p = new QLabel("[ms]");
    tLedLayout->addWidget(l->label3p, 8, 2);


    l->label4 = new QLabel("BRIGHTNESS");
    tLedLayout->addWidget(l->label4, 9, 0);
    l->brightness = new QSpinBox();
    l->brightness->setRange(0, 100);
    tLedLayout->addWidget(l->brightness, 9, 1);
    l->label4p = new QLabel("[\%]");
    tLedLayout->addWidget(l->label4p, 9, 2);


}

void BCI::clearTLeds() {
    TLed *l;
    for (int i = 0; i < tleds.size(); ++i) {
        l = tleds[i];
        delete l->label_pattern;
        delete l->pattern;
        delete l->label_bin;
        delete l->bin;
        delete l->label_wave;
        delete l->waveform;
        delete l->bin_layout;

        delete l->label1;
        delete l->label2;
        delete l->label3;
        delete l->label4;

        delete l->label2p;
        delete l->label3p;
        delete l->label4p;

        l->pattern_length->disconnect();
        delete l->pattern_length;
        delete l->pulse_length;
        delete l->pulse_skew;
        delete l->brightness;


    }
    tleds.clear();
    t_tabs->clear();

}

void BCI::tPatternChanged() {
    int index = -1;
    for (int i = 0; i < tleds.size(); ++i) {
        if (sender() == tleds[i]->pattern) {
            index = i;
            break;
        }

    }

    tleds[index]->bin->setText(tleds[index]->pattern->toNiceString()->toAscii());

    tleds[index]->waveform->setValue(tleds[index]->pattern->value());

}

void BCI::tLengthChanged() {
    int index = -1;
    for (int i = 0; i < tleds.size(); ++i) {
        if (sender() == tleds[i]->pattern_length) {
            index = i;
            break;
        }
    }
    tleds[index]->pattern->patternResize(1, tleds[index]->pattern_length->value());
    tleds[index]->bin->setText(tleds[index]->pattern->toNiceString()->toAscii());



}


const QString DATA_ROOT = "BCI";
const QString DATA_ROOT_CVEP = "C-VEP";
const QString DATA_STIMULI_COUNT = "STIMULI_COUNT";
const QString DATA_PATTERN = "PATTERN";
const QString DATA_PULSE_LENGTH = "PULSE_LENGTH";
const QString DATA_PULSE_SKEW = "PULSE_SKEW";
const QString DATA_BRIGHTNESS = "BRIGHTNESS";

const QString DATA_ROOT_FVEP = "F-VEP";
const QString DATA_LED = "LED";
const QString DATA_TIME_ON = "TIME_ON";
const QString DATA_TIME_OFF = "TIME_OFF";
const QString DATA_FREQUENCY = "FREQUENCY";
const QString DATA_DUTY_CYCLE = "DUTY_CYCLE";

const QString DATA_ROOT_TVEP = "T-VEP";


void BCI::loadFile(QString filepathname) {
    QDomDocument doc;
    QFile file(filepathname);
    QString value = "";
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomNode cvepnode = doc.elementsByTagName(DATA_ROOT_CVEP).item(0);
    q_stimuli_count->setValue(cvepnode.firstChildElement(DATA_STIMULI_COUNT).text().toInt());
    value = cvepnode.firstChildElement(DATA_PATTERN).text();
    q_pattern->fromString(value);
    q_pulse_length->setValue(cvepnode.firstChildElement(DATA_PULSE_LENGTH).text().toInt());
    q_pulse_skew->setValue(cvepnode.firstChildElement(DATA_PULSE_SKEW).text().toInt());
    q_brightness->setValue(cvepnode.firstChildElement(DATA_BRIGHTNESS).text().toInt());

    QDomNode fvepnode = doc.elementsByTagName(DATA_ROOT_FVEP).item(0);
    QDomNodeList fvepleds = fvepnode.toElement().elementsByTagName(DATA_LED);
    clearFLeds();
    for (int i = 0; i < fvepleds.size(); ++i) {
        addFLed();
        QDomNode lednode = fvepleds.item(i);
        fleds[i]->time_on->setValue(lednode.firstChildElement(DATA_TIME_ON).text().toInt());
        fleds[i]->time_off->setValue(lednode.firstChildElement(DATA_TIME_OFF).text().toInt());
        fleds[i]->frequency->setValue(lednode.firstChildElement(DATA_FREQUENCY).text().toInt());
        fleds[i]->duty_cycle->setValue(lednode.firstChildElement(DATA_DUTY_CYCLE).text().toInt());
        fleds[i]->brightness->setValue(lednode.firstChildElement(DATA_BRIGHTNESS).text().toInt());
    }

    QDomNode tvepnode = doc.elementsByTagName(DATA_ROOT_TVEP).item(0);
    QDomNodeList tvepleds = tvepnode.toElement().elementsByTagName(DATA_LED);
    clearTLeds();
    for (int i = 0; i < tvepleds.size(); ++i) {
        addTLed();
        QDomNode lednode = tvepleds.item(i);
        value = lednode.firstChildElement(DATA_PATTERN).text();
        tleds[i]->pattern_length->setValue(value.length());
        tleds[i]->pattern->fromString(value);
        tleds[i]->pulse_length->setValue(lednode.firstChildElement(DATA_PULSE_LENGTH).text().toInt());
        tleds[i]->pulse_skew->setValue(lednode.firstChildElement(DATA_PULSE_SKEW).text().toInt());
        tleds[i]->brightness->setValue(lednode.firstChildElement(DATA_BRIGHTNESS).text().toInt());
    }


    file.close();


};

void BCI::saveFile(QString filepathname) {
    //musime zobrazit vsechny prvky jinak v nich nebudou hodnoty... just QT thing
    int backup = tabs->currentIndex();
    tabs->setCurrentIndex(0);
    int backup2 = f_tabs->currentIndex();
    for (int i = 0; i < f_tabs->count(); ++i) {
        f_tabs->setCurrentIndex(i);
    }
    f_tabs->setCurrentIndex(backup2);
    tabs->setCurrentIndex(1);
    backup2 = t_tabs->currentIndex();
    for (int i = 0; i < t_tabs->count(); ++i) {
        t_tabs->setCurrentIndex(i);
    }
    t_tabs->setCurrentIndex(backup2);
    tabs->setCurrentIndex(2);
    tabs->setCurrentIndex(3);
    tabs->setCurrentIndex(backup);

    QFile file(filepathname);
    QString value = "";
    if (file.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement(DATA_ROOT);

        xml.writeStartElement(DATA_ROOT_FVEP);
        for (int i = 0; i < fleds.size(); i++) {
            xml.writeStartElement(DATA_LED);
            xml.writeTextElement(DATA_TIME_ON, fleds[i]->time_on->text());
            xml.writeTextElement(DATA_TIME_OFF, fleds[i]->time_off->text());
            xml.writeTextElement(DATA_FREQUENCY, fleds[i]->frequency->text());
            xml.writeTextElement(DATA_DUTY_CYCLE, fleds[i]->duty_cycle->text());
            xml.writeTextElement(DATA_BRIGHTNESS, fleds[i]->brightness->text());
            xml.writeEndElement();
        }
        xml.writeEndElement();

        xml.writeStartElement(DATA_ROOT_TVEP);
        for (int i = 0; i < tleds.size(); i++) {
            xml.writeStartElement(DATA_LED);
            xml.writeTextElement(DATA_PATTERN, tleds[i]->pattern->toString()->toAscii());
            xml.writeTextElement(DATA_PULSE_LENGTH, tleds[i]->pulse_length->text());
            xml.writeTextElement(DATA_PULSE_SKEW, tleds[i]->pulse_skew->text());
            xml.writeTextElement(DATA_BRIGHTNESS, tleds[i]->brightness->text());
            xml.writeEndElement();
        }
        xml.writeEndElement();


        xml.writeStartElement(DATA_ROOT_CVEP);
        xml.writeTextElement(DATA_STIMULI_COUNT, q_stimuli_count->text());
        xml.writeTextElement(DATA_PATTERN, q_pattern->toString()->toAscii());
        xml.writeTextElement(DATA_PULSE_LENGTH, q_pulse_length->text());
        xml.writeTextElement(DATA_PULSE_SKEW, q_pulse_skew->text());
        xml.writeTextElement(DATA_BRIGHTNESS, q_brightness->text());
        xml.writeEndElement();

        xml.writeEndElement();
        xml.writeEndDocument();

        file.close();
    }
}














