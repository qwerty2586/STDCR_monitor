#ifndef STIMULATOR_CONTROL_QTESTER_H
#define STIMULATOR_CONTROL_QTESTER_H

#include <QObject>

class QTester : public QObject {
Q_OBJECT

public:
    QByteArray input_buffer;

    void emit_data(QByteArray data) { emit output(data);};
    void clear() {input_buffer.clear();};
public slots:
    void input(QByteArray data);

signals:
    void output(QByteArray data);
};
#endif //STIMULATOR_CONTROL_QTESTER_H
