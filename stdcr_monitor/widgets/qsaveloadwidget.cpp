//
// Created by qwerty on 28. 3. 2016.
//

#include <QListWidget>
#include <QLabel>
#include <QDir>
#include "qsaveloadwidget.h"

QSaveLoadWidget::QSaveLoadWidget(const QString directory, const QString prefix, const QString suffix, QWidget *parent) {
    this->setParent(parent);

    this->directory = QString(directory);
    this->prefix = QString(prefix);
    this->suffix = QString(suffix);
    //name_list = QStringList();
    initItems();

}

void QSaveLoadWidget::initItems() {
    layout = new QVBoxLayout();
    this->setLayout(layout);
    list = new QListWidget();
    layout->addWidget(list, 1);
    QHBoxLayout *qhBoxLayout = new QHBoxLayout();
    prefix_label = new QLabel(prefix);
    qhBoxLayout->addWidget(prefix_label);
    name_edit = new QLineEdit();
    qhBoxLayout->addWidget(name_edit, 1); // roztahneme jmeno souboru
    suffix_label = new QLabel(suffix);
    qhBoxLayout->addWidget(suffix_label);
    load_button = new QPushButton("LOAD");
    qhBoxLayout->addWidget(load_button);
    save_button = new QPushButton("SAVE");
    qhBoxLayout->addWidget(save_button);
    delete_button = new QPushButton("DELETE");
    qhBoxLayout->addWidget(delete_button);
    qhBoxLayout->setStretch(1, 2); // roztahneme pole s nazvem souboru
    qhBoxLayout->setStretch(1, 2); // roztahneme pole s nazvem souboru
    layout->addLayout(qhBoxLayout);
    refreshList();

    save_button->setEnabled(false);
    load_button->setEnabled(
            false);
    delete_button->setEnabled(
            false);// pocatecni stav tlacitek je nestisknutelne az se zadanim prislusneho jmena se zpristupni

    connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(listItemSelected(int)));
    connect(name_edit, SIGNAL(textChanged(QString)), this, SLOT(nameEditChanged(QString)));

    connect(save_button, SIGNAL(released()), this, SLOT(saveClick()));
    connect(load_button, SIGNAL(released()), this, SLOT(loadClick()));
    connect(delete_button, SIGNAL(released()), this, SLOT(deleteClick()));

}


void QSaveLoadWidget::refreshList() {
    populateList();
    list->clear();
    list->addItems(name_list);
    nameEditChanged(name_edit->text());

}

void QSaveLoadWidget::populateList() {
    name_list.clear();
    QDir dir(directory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }  // vytvorime adresar pokud neexistuje
    QStringList filters;
    filters.push_back(prefix + "*" + suffix);
    dir.setNameFilters(filters);
    name_list = dir.entryList();

}

void QSaveLoadWidget::listItemSelected(int item) {
    if (item < 0)
        return; // zabraneni lezeni do indexu -1 v poli
    QString shortname = name_list[item];
    shortname = shortname.mid(prefix.size(), shortname.size() - prefix.size() - suffix.size());
    name_edit->setText(shortname);
}

void QSaveLoadWidget::nameEditChanged(const QString name) {
    if (name.size() <= 0) {
        load_button->setEnabled(false);
        delete_button->setEnabled(false);
        save_button->setEnabled(false);
        if (list->currentRow() >= 0) list->item(list->currentRow())->setSelected(false);
        list->setCurrentRow(-1); // odselectnuti polozky v listu
        return;
    }
    save_button->setEnabled(true);
    QString longname = prefix + name + suffix;
    for (int i = 0; i < name_list.size(); ++i) {
        if (longname == name_list[i]) {
            list->setCurrentRow(i);
            load_button->setEnabled(true);
            delete_button->setEnabled(true);
            return;
        }
    }
    if (list->currentRow() >= 0) list->item(list->currentRow())->setSelected(false);
    list->setCurrentRow(-1);
    load_button->setEnabled(false);
    delete_button->setEnabled(false);
}

void QSaveLoadWidget::saveClick() {
    QString fullpath = directory;
    if (fullpath.right(1).compare("/") != 0) fullpath += "/";
    fullpath += prefix + name_edit->text() + suffix;
    emit save(fullpath);
}

void QSaveLoadWidget::loadClick() {
    QString fullpath = directory;
    if (fullpath.right(1).compare("/") != 0) fullpath += "/";
    fullpath += prefix + name_edit->text() + suffix;
    emit load(fullpath);
}

void QSaveLoadWidget::deleteClick() {
    QString fullpath = directory;
    if (fullpath.right(1).compare("/") != 0) fullpath += "/";
    fullpath += prefix + name_edit->text() + suffix;
    QFile file(fullpath);
    file.remove();
    refreshList();
}


void QSaveLoadWidget::setPrefix(const QString prefix) {
    if (this->prefix.compare(prefix) != 0) {
        this->prefix = prefix;
        prefix_label->setText(prefix);
        refreshList();
    }
}



















