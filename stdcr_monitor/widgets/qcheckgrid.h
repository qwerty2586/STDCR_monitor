#ifndef STDCR_MONITOR_QCHECKGRID_H
#define STDCR_MONITOR_QCHECKGRID_H

#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>

class QCheckGrid : public QWidget {
Q_OBJECT

public:


    explicit QCheckGrid(int w, int h, QWidget *parent = 0);

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
