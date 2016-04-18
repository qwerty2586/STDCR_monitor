//
// Created by qwerty on 28. 3. 2016.
//

#ifndef STDCR_MONITOR_QSAVELOADWIDGET_H
#define STDCR_MONITOR_QSAVELOADWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>

class QSaveLoadWidget : public QWidget {
Q_OBJECT

public:


    explicit QSaveLoadWidget(const QString directory, const QString prefix, const QString suffix, QWidget *parent = 0);

    void setPrefix(const QString qString);

public slots:

    void refreshList();

signals:

    void save(QString);

    void load(QString);

private:

    QString directory, prefix, suffix;
    QStringList name_list;

    QVBoxLayout *layout;
    QListWidget *list;
    QPushButton *save_button, *load_button;
    QLineEdit *name_edit;
    QLabel *prefix_label, *suffix_label;

    void initItems();

private slots:

    void populateList();

    void listItemSelected(int item);

    void nameEditChanged(const QString name);

    void saveClick();

    void loadClick();
};

#endif //STDCR_MONITOR_QSAVELOADWIDGET_H
