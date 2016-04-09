#ifndef STDCR_MONITOR_SERIAL_COMM_H
#define STDCR_MONITOR_SERIAL_COMM_H

#include <QObject>
#include <c++/array>
#include "../3rdparty/qextserialport/qextserialport.h"

#define MAX_LEDS 8

class Stimulator : public QObject {
Q_OBJECT

public:
    static const char MESSAGE_TIME_ON_LED0 = 0x10;
    static const char MESSAGE_TIME_ON_OFFSET = 0x02;
    static const char MESSAGE_TIME_PAUSE_LED0 = 0x11;
    static const char MESSAGE_TIME_PAUSE_OFFSET = 0x02;
    static const char MESSAGE_DISTRIBUTION_LED[MAX_LEDS];
    static const int PACKET_SIZE = 64;
    //



    explicit Stimulator(const QString filename = 0);

    void setFile(const QString filename);

    void fileConnect();


private:
    QByteArray buffer;
    QextSerialPort *m_port;
    bool connected, opened, deviceOn;
private slots:

    void onReadyRead();

    void onDsrChanged(bool status);


    void checkBuffer();
};

//const char Stimulator::MESSAGE_DISTRIBUTION_LED[MAX_LEDS] = {0x18,0,0,0,0,0,0,0};

#endif //STDCR_MONITOR_SERIAL_COMM_H
