#ifndef STIMULATOR_CONTROL_QVERTICALSCROLLAREA_H
#define STIMULATOR_CONTROL_QVERTICALSCROLLAREA_H

#include <QScrollArea>

class QVerticalScrollArea : public QScrollArea
{
Q_OBJECT
public:
    explicit QVerticalScrollArea(QWidget *parent = 0);

    virtual bool eventFilter(QObject *o, QEvent *e) override;

};

#endif //STIMULATOR_CONTROL_QVERTICALSCROLLAREA_H
