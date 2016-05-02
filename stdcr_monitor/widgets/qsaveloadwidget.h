#ifndef STDCR_MONITOR_QSAVELOADWIDGET_H
#define STDCR_MONITOR_QSAVELOADWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>

/**
 * Widget QSaveLoadWidget prinasi uzivatelske rozhrani pro vyber souboru
 * v adresari, ktery je fitrovan pomoci prefixu a suffixu. Tlacitka SAVE a LOAD
 * emituji signaly, kterymi predavaji nazev souboru. Stisk tlacika DELETE
 * se provadi funkci widgetu a neodesila se.
 */

class QSaveLoadWidget : public QWidget {
Q_OBJECT

public:


    explicit QSaveLoadWidget(const QString directory, const QString prefix, const QString suffix, QWidget *parent = 0);

    /// zmeni prefix
    void setPrefix(const QString qString);

public slots:

    void refreshList();

signals:

    /// signal o stisku tlacitka SAVE
    void save(QString);

    /// signal o stisku tlacitka LOAD
    void load(QString);

private:

    QString directory, prefix, suffix;
    QStringList name_list;

    QVBoxLayout *layout;
    QListWidget *list;
    QPushButton *save_button, *load_button, *delete_button;
    QLineEdit *name_edit;
    QLabel *prefix_label, *suffix_label;

    void initItems();

private slots:

    void populateList();

    void listItemSelected(int item);

    void nameEditChanged(const QString name);

    void saveClick();

    void loadClick();

    void deleteClick();
};

#endif //STDCR_MONITOR_QSAVELOADWIDGET_H
