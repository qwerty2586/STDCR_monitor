
#ifndef STDCR_MONITOR_QWAVEFORM_H
#define STDCR_MONITOR_QWAVEFORM_H

#include <QWidget>

class QWaveForm : public QWidget {
Q_OBJECT

public:


    explicit QWaveForm(int length, QWidget *parent = 0);


    static const int HEIGHT = 10;

    QSize sizeHint() const { return QSize(HEIGHT * m_length, HEIGHT); }

    QSize minimumSizeHint() const { return QSize(HEIGHT * m_length, HEIGHT); }

public slots:

    void setValue(std::vector<bool> value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_length;
    std::vector<bool> m_value;

};

#endif //STDCR_MONITOR_QWAVEFORM_H
