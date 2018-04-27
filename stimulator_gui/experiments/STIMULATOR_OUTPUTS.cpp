#include <QTabWidget>
#include <QVBoxLayout>
#include <stimulator_gui/widgets/qverticalscrollarea.h>
#include <QGroupBox>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <stimulator_gui/widgets/qsaveloadwidget.h>
#include <stimulator_gui/params.h>
#include <QtXml/QDomDocument>
#include <QtCore/QXmlStreamWriter>
#include "STIMULATOR_OUTPUTS.h"

STIMULATOR_OUTPUTS::STIMULATOR_OUTPUTS(QWidget *parent) : Experiment(parent) {

    {
        LINEEDIT_PALLETE = QLineEdit().palette();
        LINEEDIT_READONLY_PALLETE = QLineEdit().palette();
        LINEEDIT_READONLY_PALLETE.setColor(QPalette::Base,LINEEDIT_READONLY_PALLETE.color(QPalette::Button));
        LINEEDIT_READONLY_PALLETE.setColor(QPalette::Text,LINEEDIT_READONLY_PALLETE.color(QPalette::HighlightedText));
        // palety pro neditovatelny LineEdit
    }
    initItems();
}

void STIMULATOR_OUTPUTS::initItems() {

    QVBoxLayout *l = new QVBoxLayout();

    QVerticalScrollArea *scrollArea = new QVerticalScrollArea();

    tabs = new QTabWidget();
    tabs->addTab(scrollArea, "OUTPUTS SETUP");
    tabs->addTab(new QWidget(), "SCHEMA");
    l->addWidget(tabs);
    this->setLayout(l);


    QVBoxLayout *outputs_layout = new QVBoxLayout();
    QWidget *scrolling_widget = new QWidget();
    scrolling_widget->setLayout(outputs_layout);


    for (int i = 0; i < LEDS_COUNT; ++i) {


        Output *item = new Output();
        outputs.push_back(item);

        QGridLayout *item_grid = new QGridLayout();
        outputs_layout->addLayout(item_grid);
        item->label = new QLabel(QString("<b>OUTPUT ").append(QString::number(i)).append("</b>"));

        item_grid->addWidget(item->label, 0, 0, 1, 1);

        QHBoxLayout *group_hbox = new QHBoxLayout();
        QGroupBox *group_box = new QGroupBox();
        group_box->setFlat(true); // udelame maly groupbox
        group_box->setStyleSheet("margin:0;padding:0;border:0;");
        group_box->setAlignment(Qt::AlignLeft);
        item->radio_led = new QRadioButton("LED");
        item->radio_image = new QRadioButton("IMAGE");
        item->radio_audio = new QRadioButton("AUDIO");
        group_hbox->addWidget(item->radio_led);
        group_hbox->addWidget(item->radio_image);
        group_hbox->addWidget(item->radio_audio);
        group_box->setLayout(group_hbox);
        group_box->setMaximumWidth(group_box->width());
        item->radio_image->setChecked(true);
        item_grid->addWidget(group_box, 0, 1, 1, 1);

        item->path_line = new QLineEdit();
        item->path_line->setStyleSheet("padding-bottom: 0px; margin-bottom: 0px;");
        item_grid->addWidget(item->path_line, 1, 0, 1, 2);


        connect(item->radio_led,SIGNAL(toggled(bool)),this,SLOT(outputs_changed(bool)));
        connect(item->radio_image,SIGNAL(toggled(bool)),this,SLOT(outputs_changed(bool)));
        connect(item->radio_audio,SIGNAL(toggled(bool)),this,SLOT(outputs_changed(bool)));

        item->image = new QLabel();
        item->image->setFixedSize(75, 75);
        item->image->setStyleSheet("background: #FFF");
        item->image->setAlignment(Qt::AlignCenter);
        item->image->setPixmap(QPixmap::fromImage(QImage(":/res/image.png")));
        item_grid->addWidget(item->image, 0, 2, 2, 1);
        item->path_line->setReadOnly(false);
        item->path_line->setPalette(LINEEDIT_READONLY_PALLETE);


        item_grid->setRowStretch(0, 10);
        item_grid->setRowStretch(1, 0);

        if (i != LEDS_COUNT - 1) {
            QFrame *line = new QFrame();
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            outputs_layout->addWidget(line);
        }
    }

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrolling_widget);

    QGridLayout *schemaLayout = new QGridLayout();
    tabs->widget(1)->setLayout(schemaLayout);
    QSaveLoadWidget *qSaveLoadWidget = new QSaveLoadWidget(OUTPUTS_DIR, "", OUTPUTS_EXTENSION);
    schemaLayout->addWidget(qSaveLoadWidget, 0, 0);
    connect(qSaveLoadWidget, SIGNAL(load(QString)), this, SLOT(loadFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), this, SLOT(saveFile(QString)));
    connect(qSaveLoadWidget, SIGNAL(save(QString)), qSaveLoadWidget, SLOT(refreshList()));



}

void STIMULATOR_OUTPUTS::outputs_changed(bool output_enable) {
    if (!output_enable)
        return; // odstrani dvojita volani

    int sender_index = 0;
    for (int i = 0; i < LEDS_COUNT; ++i) {
        if (sender() == outputs[i]->radio_led || sender() == outputs[i]->radio_image ||
            sender() == outputs[i]->radio_audio) {
            sender_index = i;
            break;
        }
    }
    if (outputs[sender_index]->radio_led->isChecked()) {
        outputs[sender_index]->image->setPixmap(QPixmap::fromImage(QImage(":/res/led.png")));
        outputs[sender_index]->path_line->setReadOnly(true);
    }
    if (outputs[sender_index]->radio_image->isChecked()) {
        outputs[sender_index]->image->setPixmap(QPixmap::fromImage(QImage(":/res/image.png")));
        outputs[sender_index]->path_line->setReadOnly(false);
    }
    if (outputs[sender_index]->radio_audio->isChecked()) {
        outputs[sender_index]->image->setPixmap(QPixmap::fromImage(QImage(":/res/audio.png")));
        outputs[sender_index]->path_line->setReadOnly(false);
    }

    if (outputs[sender_index]->path_line->isReadOnly()) {
        outputs[sender_index]->path_line->setPalette(LINEEDIT_READONLY_PALLETE);
    } else {
        outputs[sender_index]->path_line->setPalette(LINEEDIT_PALLETE);
    }
}

const QString DATA_ROOT = "outputs";
const QString DATA_CHILD = "output";
const QString DATA_CHILD_FILENAME = "filename";
const QString DATA_CHILD_TYPE = "type";
const QString DATA_CHILD_TYPE_UNKNOWN = "UNKNOWN";
const QString DATA_CHILD_TYPE_IMAGE = "IMAGE";
const QString DATA_CHILD_TYPE_AUDIO = "AUDIO";
const QString DATA_CHILD_TYPE_VIDEO = "VIDEO";
const QString DATA_CHILD_TYPE_OTHER = "OTHER";


void STIMULATOR_OUTPUTS::loadFile(QString filepathname) {
    QDomDocument doc;
    QFile file(filepathname);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return;

    QDomNode root = doc.elementsByTagName(DATA_ROOT).item(0);
    QDomNodeList xml_outputs = doc.elementsByTagName(DATA_CHILD);

    for (int i = 0; i < outputs.size(); i++) {
        QDomNode output = xml_outputs.item(i);
        outputs[i]->path_line->setText(output.firstChildElement(DATA_CHILD_FILENAME).text());
        QString type = output.firstChildElement(DATA_CHILD_TYPE).text();
        if (type == DATA_CHILD_TYPE_IMAGE) {
            outputs[i]->path_line->setReadOnly(false);
            outputs[i]->path_line->setText(output.firstChildElement(DATA_CHILD_FILENAME).text());
            outputs[i]->radio_image->setChecked(true);
        } else if (type == DATA_CHILD_TYPE_AUDIO) {
            outputs[i]->path_line->setReadOnly(false);
            outputs[i]->path_line->setText(output.firstChildElement(DATA_CHILD_FILENAME).text());
            outputs[i]->radio_audio->setChecked(true);
        } else {
            outputs[i]->path_line->setReadOnly(true);
            outputs[i]->path_line->setText("");
            outputs[i]->radio_led->setChecked(true);
        }

        if (outputs[i]->path_line->isReadOnly()) {
            outputs[i]->path_line->setPalette(LINEEDIT_READONLY_PALLETE);
        } else {
            outputs[i]->path_line->setPalette(LINEEDIT_PALLETE);
        }
    }
    file.close();

}

void STIMULATOR_OUTPUTS::saveFile(QString filepathname) {
    //musime zobrazit vsechny prvky jinak v nich nebudou hodnoty... just QT thing
    int backup = tabs->currentIndex();
    tabs->setCurrentIndex(1);
    tabs->setCurrentIndex(2);
    tabs->setCurrentIndex(backup);
    QFile file(filepathname);
    
    if (file.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter xml(&file);
        xml.setAutoFormatting(true);
        xml.writeStartDocument();
        xml.writeStartElement(DATA_ROOT);
        for (auto &output : outputs) {
            xml.writeStartElement(DATA_CHILD);
            if (output->radio_led->isChecked()) {
                xml.writeTextElement(DATA_CHILD_TYPE,DATA_CHILD_TYPE_OTHER);
            }
            if (output->radio_audio->isChecked()){
                xml.writeTextElement(DATA_CHILD_TYPE,DATA_CHILD_TYPE_AUDIO);
                xml.writeTextElement(DATA_CHILD_FILENAME, output->path_line->text());
            }
            if (output->radio_image->isChecked()){
                xml.writeTextElement(DATA_CHILD_TYPE,DATA_CHILD_TYPE_IMAGE);
                xml.writeTextElement(DATA_CHILD_FILENAME, output->path_line->text());
            }
            xml.writeEndElement();
        }

        xml.writeEndElement();
        xml.writeEndDocument();

        file.close();
    }

}
