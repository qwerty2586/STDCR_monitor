#include <QTabWidget>
#include <QVBoxLayout>
#include <stimulator_gui/widgets/qverticalscrollarea.h>
#include <QGroupBox>
#include <QFile>
#include <QDebug>
#include <QDir>
#include "STIMULATOR_OUTPUTS.h"

STIMULATOR_OUTPUTS::STIMULATOR_OUTPUTS(QWidget *parent) : Experiment(parent) {

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
        item->radio_led->setChecked(true);
        item_grid->addWidget(group_box, 0, 1, 1, 1);

        QLineEdit *path_line = new QLineEdit();
        path_line->setStyleSheet("padding-bottom: 0px; margin-bottom: 0px;");
        item_grid->addWidget(path_line, 1, 0, 1, 2);

        item->image = new QLabel();
        item->image->setFixedSize(75, 75);
        item->image->setStyleSheet("background: #FFF");
        item->image->setAlignment(Qt::AlignCenter);
        item->image->setPixmap(QPixmap::fromImage(QImage(":/res/led.png")));
        item_grid->addWidget(item->image, 0, 2, 2, 1);


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



}
