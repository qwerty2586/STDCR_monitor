#ifndef STDCR_MONITOR_SERIAL_COMM_H
#define STDCR_MONITOR_SERIAL_COMM_H

#include <QObject>


class Stimulator : public QObject {
Q_OBJECT

public:
    static const char MESSAGE_TIME_ON_LED0 = 0x10;
    static const char MESSAGE_TIME_ON_OFFSET = 0x02;
    static const char MESSAGE_TIME_PAUSE_LED0 = 0x11;
    static const char MESSAGE_TIME_PAUSE_OFFSET = 0x02;
    static const char MESSAGE_DISTRIBUTION_LED0 = 0x18;
    static const char MESSAGE_DISTRIBUTION_OFFSET = 0x01;


    explicit Stimulator(const QString filename = 0);

    void setFile(const QString filename);

    void fileConnect();


private:
    QByteArray buffer;
    QextSerialPort m_port;
    bool connected;



};

#endif //STDCR_MONITOR_SERIAL_COMM_H
