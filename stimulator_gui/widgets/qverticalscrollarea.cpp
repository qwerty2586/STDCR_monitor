#include <QEvent>
#include <QScrollBar>

#include "qverticalscrollarea.h"

QVerticalScrollArea::QVerticalScrollArea(QWidget *parent) :
        QScrollArea(parent)
{
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

bool QVerticalScrollArea::eventFilter(QObject *o, QEvent *e)
{
    if(o && o == widget() && e->type() == QEvent::Resize)
        setMinimumWidth(widget()->minimumSizeHint().width() + verticalScrollBar()->width());

    return QScrollArea::eventFilter(o, e);
}


