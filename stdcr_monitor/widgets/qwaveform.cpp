//
// Created by qwerty on 26. 3. 2016.
//

#include <QPainter>
#include "qwaveform.h"


QWaveForm::QWaveForm(int length, QWidget *parent) {
    this->setParent(parent);
    m_length = length;
    this->resize(HEIGHT * length, HEIGHT);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    for (int i = 0; i < length; ++i) {
        m_value.push_back(false);
    }

}

void QWaveForm::setValue(std::vector<bool> value) {
    m_value = value;

    this->update();
}

void QWaveForm::paintEvent(QPaintEvent *event) {

    int LOW = HEIGHT - 1;
    int HIGH = 0;

    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::black), 1));

    for (int i = 0; i < m_length; ++i) {
        if (m_value[i])
            painter.drawLine(i * HEIGHT, HIGH, (i + 1) * HEIGHT, HIGH);
        else
            painter.drawLine(i * HEIGHT, LOW, (i + 1) * HEIGHT, LOW);

        if (i > 0) {
            if (m_value[i - 1] != m_value[i])
                painter.drawLine(i * HEIGHT, LOW, i * HEIGHT, HIGH);

        }

    }
};



