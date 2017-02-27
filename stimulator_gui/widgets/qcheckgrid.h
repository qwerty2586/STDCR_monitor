#ifndef STDCR_MONITOR_QCHECKGRID_H
#define STDCR_MONITOR_QCHECKGRID_H

#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>

/**
 * Widget QCheckGrid slouzi k vizualizaci a zmenam bitoveho pole. Mrizku s widgety je mozno
 * rozlozit na vice radku. Po vytvoreni je mozne mrizce zmenit velikost.
 */

class QCheckGrid : public QWidget {
Q_OBJECT


public:


    explicit QCheckGrid(int rows, int columns, QWidget *parent = 0);

    /// zmeni velikost mrizky, pokud se neco prida, nove boxy nebudou zasktrnute, pokud se zkrati tak se prebytecna data ztrati
    void patternResize(int rows, int columns);

    /// hodnota bitoveho pole
    std::vector<bool> value();

    /// vrati bitove pole jako retezec 1 a 0 odsazene po 4 cifrach
    QString *toNiceString();

    /// vrati bitove pole jako retezec 1 a 0
    QString *toString();

    /// nacte bitove pole z retezce 1 a 0
    void fromString(QString s);
signals:

    /// signalizuje zmenu hodnoty, treba pri stisku tlacitka
    void valueChanged();

private:
    int width, height;
    std::vector<QCheckBox *> boxes;
    std::vector<bool> m_value;
    QGridLayout *layout;

    void setBox(int box, bool b);

private slots:

    void boxChange();

};

#endif //STDCR_MONITOR_QCHECKGRID_H
