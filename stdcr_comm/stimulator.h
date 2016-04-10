#ifndef STDCR_MONITOR_SERIAL_COMM_H
#define STDCR_MONITOR_SERIAL_COMM_H

#include <QObject>
#include "../3rdparty/qextserialport/qextserialport.h"

#define LEDS_COUNT 8

class Stimulator : public QObject {
Q_OBJECT

public:
    static constexpr char MESSAGE_TIME_ON_LED[LEDS_COUNT] = {0x10, 0x12, 0x14, 0x16, 0x2C, 0x2E, 0x30, 0x32};
    static constexpr char MESSAGE_TIME_PAUSE_LED[LEDS_COUNT] = {0x11, 0x13, 0x15, 0x17, 0x2D, 0x2F, 0x31, 0x33};
    static constexpr char MESSAGE_DISTRIBUTION_LED[LEDS_COUNT] = {0x18, 0x19, 0x1A, 0x1B, 0x34, 0x35, 0x36, 0x37};
    static constexpr char MESSAGE_BRIGHTNESS_LED[LEDS_COUNT] = {0x1C, 0x1D, 0x1E, 0x1F, 0x38, 0x38, 0x39,
                                                              0x39}; // je spolecny pro led56 a led78 ?
    static constexpr char MESSAGE_FREQUENCY[LEDS_COUNT] = {0x22, 0x24, 0x26, 0x28, 0x3A, 0x3C, 0x3E, 0x40};

    static constexpr char MESSAGE_SYNC_CYCLE = 0x20;
    static constexpr char MESSAGE_PULSE_EDGE = 0x21;

    static const int PACKET_SIZE = 64;




    explicit Stimulator(const QString filename = 0);

    void setFile(const QString filename);

    void fileConnect();

    void portDisconnect();

    bool isPortConnected();

signals:

    void incomingMessage(char, QByteArray);

    void connected(bool);


private:
    QByteArray buffer, incoming_data;
    char incoming_type;
    QextSerialPort *m_port;
    bool opened, deviceOn;
private slots:

    void onReadyRead();

    void onDsrChanged(bool status);


    void checkBuffer();
};

//const int Stimulator::MESSAGE_DISTRIBUTION_LED[LEDS_COUNT];

#endif //STDCR_MONITOR_SERIAL_COMM_H
