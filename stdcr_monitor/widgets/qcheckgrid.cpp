#include "qcheckgrid.h"


const static int QCHECKGRID_MARGIN = 3;


QCheckGrid::QCheckGrid(int rows, int columns, QWidget *parent) {
    int count = columns * rows;
    layout = new QGridLayout();
    layout->setSpacing(0);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    this->setLayout(layout);
    for (int i = 0; i < count; i++) {
        boxes.push_back(new QCheckBox);
        m_value.push_back(false);
        boxes[i]->setChecked(false);
        layout->addWidget(boxes[i], i / columns, i % columns);
        connect(boxes[i], SIGNAL(stateChanged(int)), this, SLOT(boxChange()));
    }
    layout->setMargin(QCHECKGRID_MARGIN);


}

void QCheckGrid::setBox(int box, bool b) {
    m_value[box] = b;
    boxes[box]->setChecked(b);
}


void QCheckGrid::boxChange() {

    for (int i = 0; i < boxes.size(); i++) {
        if (boxes[i] == sender()) {
            m_value[i] = boxes[i]->isChecked();
        }
    }
    emit valueChanged();
}

std::vector<bool> QCheckGrid::value() {
    return m_value;
}

QString *QCheckGrid::toNiceString() {

    QString *s = new QString("");
    for (int i = 0; i < m_value.size(); ++i) {
        if (m_value[i]) s->append("1");
        else s->append("0");
        if ((i + 1) % 4 == 0) s->append(" ");
    }

    return s;
}

void QCheckGrid::patternResize(int rows, int columns) {
    int count = rows * columns;
    if (count > m_value.size()) { //pridavame
        while (count != m_value.size()) {
            m_value.push_back(false);
            boxes.push_back(new QCheckBox());
            boxes[boxes.size() - 1]->setChecked(false);
            connect(boxes[boxes.size() - 1], SIGNAL(stateChanged(int)), this, SLOT(boxChange()));
        }
    }

    if (count < m_value.size()) { //ubirame
        while (count != m_value.size()) {
            m_value.pop_back();
            boxes[boxes.size() - 1]->disconnect();
            delete boxes[boxes.size() - 1];
            boxes.pop_back();
        }
    }

    for (int i = layout->count(); i >= 0; i--) {
        layout->removeItem(layout->takeAt(i));
    }

    for (int i = 0; i < count; ++i) {
        layout->addWidget(boxes[i], i / columns, i % columns);
    }


}






