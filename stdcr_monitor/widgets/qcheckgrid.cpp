#include "qcheckgrid.h"


const static int QCHECKGRID_MARGIN = 3;


QCheckGrid::QCheckGrid(int rows, int columns, QWidget *parent) {
    int count = columns * rows;
    layout = new QGridLayout();
    layout->setSpacing(0);

    this->setLayout(layout);
    for (int i = 0; i < count; i++) {
        boxes.push_back(new QCheckBox);
        m_value.push_back(false);
        boxes[i]->setChecked(false);
        layout->addWidget(boxes[i], i / columns, i % columns);
        connect(boxes[i], SIGNAL(stateChanged(int)), this, SLOT(boxChange()));
    }
    layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, columns, rows, 1);
    layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding), rows, 0, 1, columns);
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


