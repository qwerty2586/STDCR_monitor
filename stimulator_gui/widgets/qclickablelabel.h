// from
#ifndef STIMULATOR_CONTROL_QCLICKABLELABEL_H
#define STIMULATOR_CONTROL_QCLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class QClickableLabel : public QLabel {
Q_OBJECT

public:
    explicit QClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~QClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

};

#endif //STIMULATOR_CONTROL_QCLICKABLELABEL_H
