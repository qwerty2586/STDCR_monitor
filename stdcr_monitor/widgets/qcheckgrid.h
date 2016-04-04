#ifndef STDCR_MONITOR_QCHECKGRID_H
#define STDCR_MONITOR_QCHECKGRID_H

#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>

class QCheckGrid : public QWidget {
Q_OBJECT


public:


    explicit QCheckGrid(int rows, int columns, QWidget *parent = 0);


    void patternResize(int rows, int columns);
    std::vector<bool> value();

    QString *toNiceString();

    QString *toString();

    void fromString(QString s);
signals:

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
