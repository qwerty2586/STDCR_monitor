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
#include <QFileDialog>
#include <QtGui/QDesktopServices>
#include <QtGui/QImageReader>

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
    tabs->addTab(new QWidget(), "SETTINGS");
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
        item->radio_led->setChecked(true);
        item_grid->addWidget(group_box, 0, 1, 1, 1);

        QHBoxLayout *path_box =  new QHBoxLayout();
        item->load_button = new QPushButton();
        item->load_button->setIcon(QIcon(QPixmap::fromImage(QImage(":/res/open.png"))));
        item->load_button->setIconSize(QImage(":/res/open.png").size());
        path_box->addWidget(item->load_button);
        item->path_line = new QLineEdit();
        item->path_line->setStyleSheet("padding-bottom: 0px; margin-bottom: 0px;");
        path_box->addWidget(item->path_line);

        item_grid->addLayout(path_box, 1, 0, 1, 2);


        connect(item->radio_led,SIGNAL(toggled(bool)),this,SLOT(outputs_changed(bool)));
        connect(item->radio_image,SIGNAL(toggled(bool)),this,SLOT(outputs_changed(bool)));
        connect(item->radio_audio,SIGNAL(toggled(bool)),this,SLOT(outputs_changed(bool)));

        item->image = new QClickableLabel();
        item->image->setFixedSize(75, 75);
        item->image->setStyleSheet("background: #FFF");
        item->image->setAlignment(Qt::AlignCenter);
        item->image->setPixmap(QPixmap::fromImage(QImage(":/res/led.png")));
        item_grid->addWidget(item->image, 0, 2, 2, 1);
        item->path_line->setPalette(LINEEDIT_READONLY_PALLETE);
        item->path_line->setReadOnly(true);
        item->load_button->setEnabled(false);

        connect(item->load_button,SIGNAL(clicked()),this,SLOT(load_file_clicked()));
        connect(item->image,SIGNAL(clicked()),this,SLOT(image_clicked()));
        connect(item->path_line,SIGNAL(textChanged(const QString &)),this,SLOT(edit_changed()));


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

    // settings
    QGridLayout *settingsLayout = new QGridLayout();
    tabs->widget(1)->setLayout(settingsLayout);
    settings.save = new QPushButton("SAVE SETTINGS");
    settings.enabled = new QCheckBox("ENABLED");
    settings.enabled->setChecked(Params::sdlEnabled());

    QHBoxLayout *fullscreenLayout = new QHBoxLayout();
    QGroupBox *fullscreenGroup = new QGroupBox();
    fullscreenGroup->setFlat(true);
    fullscreenGroup->setStyleSheet("margin:0;padding:0;border:0;");
    fullscreenGroup->setAlignment(Qt::AlignLeft);
    settings.fullscreen = new QRadioButton("FULLSCREEN");
    settings.windowed = new QRadioButton("WINDOW");
    fullscreenLayout->addWidget(settings.fullscreen);
    fullscreenLayout->addWidget(settings.windowed);
    fullscreenGroup->setLayout(fullscreenLayout);
    if (Params::sdlFullscreen()) settings.fullscreen->setChecked(true);
    else settings.windowed->setChecked(true);

    QHBoxLayout *hwLayout = new QHBoxLayout();
    QGroupBox *hwGroup = new QGroupBox();
    hwGroup->setFlat(true);
    hwGroup->setStyleSheet("margin:0;padding:0;border:0;");
    hwGroup->setAlignment(Qt::AlignLeft);
    settings.hw = new QRadioButton("HW Renderer");
    settings.sw = new QRadioButton("SW BlitSurface");
    hwLayout->addWidget(settings.hw);
    hwLayout->addWidget(settings.sw);
    hwGroup->setLayout(hwLayout);
    if (Params::sdlHWMode()) settings.hw->setChecked(true);
    else settings.sw->setChecked(true);

    settings.width = new QSpinBox();
    settings.width->setRange(1,9999);
    settings.width->setValue(Params::sdlWidth());
    settings.height = new QSpinBox();
    settings.height->setRange(1,9999);
    settings.height->setValue(Params::sdlHeight());

    settings.debug = new QCheckBox("WHITE SQUARE");
    settings.debug->setChecked(Params::sdlDebug());

    settingsLayout->addWidget(new QLabel("SDL OUTPUT"),0,0);
    settingsLayout->addWidget(settings.enabled,0,1);
    settingsLayout->addWidget(new QLabel("SCREEN MODE"),1,0);
    settingsLayout->addWidget(fullscreenGroup,1,1);
    settingsLayout->addWidget(new QLabel("WIDTH"),2,0);
    settingsLayout->addWidget(settings.width);
    settingsLayout->addWidget(new QLabel("HEIGHT"),3,0);
    settingsLayout->addWidget(settings.height);
    settingsLayout->addWidget(new QLabel("DEBUG"),4,0);
    settingsLayout->addWidget(settings.debug);
    settingsLayout->addWidget(new QLabel("RENDER MODE"),5,0);
    settingsLayout->addWidget(hwGroup,5,1);
    settingsLayout->addWidget(settings.save,6,0,1,2);

    connect(settings.save, SIGNAL(clicked()), this, SLOT(saveSettings()));

   //  save load
    QGridLayout *schemaLayout = new QGridLayout();
    tabs->widget(2)->setLayout(schemaLayout);
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
        outputs[sender_index]->load_button->setEnabled(false);
        outputs[sender_index]->previewing = false;
    }
    if (outputs[sender_index]->radio_image->isChecked()) {
        outputs[sender_index]->image->setPixmap(QPixmap::fromImage(QImage(":/res/image.png")));
        outputs[sender_index]->path_line->setReadOnly(false);
        outputs[sender_index]->load_button->setEnabled(true);
    }
    if (outputs[sender_index]->radio_audio->isChecked()) {
        outputs[sender_index]->image->setPixmap(QPixmap::fromImage(QImage(":/res/audio.png")));
        outputs[sender_index]->path_line->setReadOnly(false);
        outputs[sender_index]->load_button->setEnabled(true);
        outputs[sender_index]->previewing = false;
    }

    outputs[sender_index]->path_line->setText("");

    if (outputs[sender_index]->path_line->isReadOnly()) {
        outputs[sender_index]->path_line->setPalette(LINEEDIT_READONLY_PALLETE);
    } else {
        outputs[sender_index]->path_line->setPalette(LINEEDIT_PALLETE);
    }
}


void STIMULATOR_OUTPUTS::load_file_clicked() {
    int sender_index = 0;
    for (int i = 0; i < LEDS_COUNT; ++i) {
        if (sender() == outputs[i]->load_button) {
            sender_index = i;
            break;
        }
    }
    if (outputs[sender_index]->radio_led->isChecked()) {
        return;
    }

    if (outputs[sender_index]->radio_image->isChecked()) {
        QString filename = QFileDialog::getOpenFileName(this,"Select image file...",FILESERVER_DIR,"Images (*.bmp *.jpg *.png);;All files (*.*)");
        QString relfilename = QDir(FILESERVER_DIR).relativeFilePath(filename);
        if (!relfilename.contains(QDir::separator())) {
            filename = relfilename;
        }
        outputs[sender_index]->path_line->setText(filename);
    }

    if (outputs[sender_index]->radio_audio->isChecked()) {
        QString filename = QFileDialog::getOpenFileName(this,"Select audio file...",FILESERVER_DIR,"Audio files (*.wav *.mp3 *.ogg *.flac);;All files (*.*)");
        QString relfilename = QDir(FILESERVER_DIR).relativeFilePath(filename);
        if (!relfilename.contains(QDir::separator())) {
            filename = relfilename;
        }
        outputs[sender_index]->path_line->setText(filename);
    }

}


void STIMULATOR_OUTPUTS::image_clicked() {
    int sender_index = 0;
    for (int i = 0; i < LEDS_COUNT; ++i) {
        if (sender() == outputs[i]->image) {
            sender_index = i;
            break;
        }
    }
    QString filename = outputs[sender_index]->path_line->text();
    if (filename.isEmpty()) {
        outputs[sender_index]->load_button->click();
        return;
    }
    if (!filename.contains(QDir::separator())) {
        QString relpath = FILESERVER_DIR;
        relpath.append(filename);
        filename = QDir(relpath).absolutePath();
    }
    QFileInfo info(filename);
    if (info.exists() && info.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filename));
    }

}

void STIMULATOR_OUTPUTS::edit_changed() {
    int sender_index = 0;
    for (int i = 0; i < LEDS_COUNT; ++i) {
        if (sender() == outputs[i]->path_line) {
            sender_index = i;
            break;
        }
    }
    if (!outputs[sender_index]->radio_image) {
        return;
    }
    QString filename = outputs[sender_index]->path_line->text();
    if (!filename.contains(QDir::separator())) {
        QString relpath = FILESERVER_DIR;
        relpath.append(filename);
        filename = QDir(relpath).absolutePath();
    }
    QFileInfo info(filename);
    if (info.exists() && info.isFile()) {
        QByteArray imageformat = QImageReader::imageFormat(filename);
        if (imageformat.length()<1) {
            if (outputs[sender_index]->previewing) {
                outputs[sender_index]->image->setPixmap(QPixmap::fromImage(QImage(":/res/image.png")));
                outputs[sender_index]->previewing = false;
            }
            return;
        }
        outputs[sender_index]->image->setPixmap(QPixmap::fromImageReader(new QImageReader(filename,imageformat)).scaled(75,75,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        outputs[sender_index]->previewing = true;
    } else {
        if (outputs[sender_index]->previewing) {
            outputs[sender_index]->image->setPixmap(QPixmap::fromImage(QImage(":/res/image.png")));
            outputs[sender_index]->previewing = false;
        }
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
        QDomNamedNodeMap attrs = output.attributes();
        outputs[i]->path_line->setText(attrs.namedItem(DATA_CHILD_FILENAME).nodeValue());
        QString type = attrs.namedItem(DATA_CHILD_TYPE).nodeValue();
        if (type == DATA_CHILD_TYPE_IMAGE) {
            outputs[i]->radio_image->setChecked(true);
            outputs[i]->path_line->setReadOnly(false);
            outputs[i]->path_line->setText(attrs.namedItem(DATA_CHILD_FILENAME).nodeValue());
        } else if (type == DATA_CHILD_TYPE_AUDIO) {
            outputs[i]->radio_audio->setChecked(true);
            outputs[i]->path_line->setReadOnly(false);
            outputs[i]->path_line->setText(attrs.namedItem(DATA_CHILD_FILENAME).nodeValue());
        } else {
            outputs[i]->radio_led->setChecked(true);
            outputs[i]->path_line->setReadOnly(true);
            outputs[i]->path_line->setText("");
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
    tabs->setCurrentIndex(3);
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
                xml.writeAttribute(DATA_CHILD_TYPE,DATA_CHILD_TYPE_OTHER);
            }
            if (output->radio_audio->isChecked()){
                xml.writeAttribute(DATA_CHILD_TYPE,DATA_CHILD_TYPE_AUDIO);
                xml.writeAttribute(DATA_CHILD_FILENAME, output->path_line->text());
            }
            if (output->radio_image->isChecked()){
                xml.writeAttribute(DATA_CHILD_TYPE,DATA_CHILD_TYPE_IMAGE);
                xml.writeAttribute(DATA_CHILD_FILENAME, output->path_line->text());
            }
            xml.writeEndElement();
        }

        xml.writeEndElement();
        xml.writeEndDocument();

        file.close();
    }

}

void STIMULATOR_OUTPUTS::saveSettings() {
    Params::setSdlEnabled(settings.enabled->isChecked());
    Params::setSdlFullscreen(settings.fullscreen->isChecked());
    Params::setSdlDebug(settings.debug->isChecked());
    Params::setSdlWidth(settings.width->value());
    Params::setSdlHeight(settings.height->value());
    Params::setSdlHWMode(settings.hw->isChecked());
}

void STIMULATOR_OUTPUTS::enterExperiment() {
    Experiment::enterExperiment();
    QFileInfo info(QDir(FILESERVER_DIR),AUTOSAVE_FILE);
    if (info.exists() && info.isReadable()) {
        loadFile(info.filePath());
    }
}

void STIMULATOR_OUTPUTS::leaveExperiment() {
    Experiment::leaveExperiment();
    saveFile(QFileInfo(QDir(FILESERVER_DIR),AUTOSAVE_FILE).filePath());
}

