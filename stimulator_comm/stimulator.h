#ifndef STDCR_MONITOR_SERIAL_COMM_H
#define STDCR_MONITOR_SERIAL_COMM_H

#include <QObject>
#include <QSerialPort>

#define LEDS_COUNT 8
/**
 * namespace StimulatorMessage shrnuje konstanty zprav pouzivanych v komunikacnim protokolu
 */
namespace StimulatorMessage {
    static constexpr char REFRESH_DISPLAY = 0x00;
    
    static constexpr char LED_ENABLE = 0x01;
    static constexpr char LED_DISABLE = 0x02;
    
    static constexpr char LED_STATUS = 0x03;


    static constexpr char TIME_ON_LED[LEDS_COUNT] = {0x10, 0x12, 0x14, 0x16, 0x2C, 0x2E, 0x30, 0x32};
    static constexpr char TIME_PAUSE_LED[LEDS_COUNT] = {0x11, 0x13, 0x15, 0x17, 0x2D, 0x2F, 0x31, 0x33};
    static constexpr char DISTRIBUTION_LED[LEDS_COUNT] = {0x18, 0x19, 0x1A, 0x1B, 0x34, 0x35, 0x36, 0x37};
    static constexpr char BRIGHTNESS_LED[LEDS_COUNT] = {0x1C, 0x1D, 0x1E, 0x1F, 0x38, 0x38, 0x39,
                                                        0x39}; // je spolecny pro led56 a led78 ?
    static constexpr char FREQUENCY[LEDS_COUNT] = {0x22, 0x24, 0x26, 0x28, 0x3A, 0x3C, 0x3E, 0x40};

    static constexpr char SYNC_CYCLE = 0x20;
    static constexpr char PULSE_EDGE = 0x21;
    static constexpr char PULSE_EDGE_UP = 0x00;
    static constexpr char PULSE_EDGE_DOWN = 0x01;
    static constexpr char SYNC_PULSE_INTERVAL = 0x20;

    static constexpr char COMMUNICATION_CODE = 0xEF;


         static constexpr char RANDOM_ENABLE = 0x04;
    static constexpr char RANDOM_DISABLE = 0x05;

}

/**
 * Trida Stimulator zjednodusuje komunikaci se stimulatorem. Odesilane zpravy se obaluji do paketu.
 * Prijate bajty se shromazduji do bufferu a kdyz je buffer delsi nez delka paketu tyto bajty se vyjmou a odeberou se
 * prebytecna data a vyvola se signal, kterym se predavaji data programu.
 */
class Stimulator : public QObject {
Q_OBJECT


public:
    ///standartni delka packetu v komunikacnim protokolu
    static const int PACKET_SIZE = 64;

    explicit Stimulator();

    // nastaveni portu
    void setPort(QSerialPortInfo &info, qint32 baudrate);

    // vrati seznam dostupnych seriovych portu
    QList<QSerialPortInfo> getPortsList();

    // vrati seznam podporovanych rychlosti
    QList<qint32> getBaudrates();

    ///pripojeni k souboru
    void portConnect();

    /// odpojeni od souboru
    void portDisconnect();

    /// vraci stav pripojeni
    bool isPortConnected();


    void sendMessage(char messageType);

    void sendMessage(char messageType, char messageData);

    void sendMessage(char messageType, int messageData);

    /// odesle data do souboru
    void sendMessage(char messageType, QByteArray messageData);

public slots:

    // fileserver pouziva signaly namisto volani
    void sendFileserverMessage(QByteArray message);

signals:

    /// notifukuje o prijeti zpravy
    void incomingMessage(char, QByteArray);

    /// notifukuje o prijeti zpravy s daty pro fileserver
    void incomingFileserverMessage(QByteArray);

    /// notikuje o uspesnem pripojeni odpojeni
    void connected(bool);


private:

    /// odchozi buffer
    QByteArray buffer;

    /// prichozi buffer
    QByteArray incoming_data;

    /// typ prichozi zpravy - viz namespace StimulatorMessage
    char incoming_type;

    /// instance tridy QextSerialPort zajistujici obsluhu serioveho portu
    QSerialPort *m_port;

    /// stav pripojeni k seriovemu portu
    bool opened;

    /// stav ochoziho zarizeni - zda je k portu pripojene zarizeni
    bool deviceOn;

private slots:

    /// obsluha prichozich dat pri jakekoli prichozi komunikaci
    void onReadyRead();

    /// zmena stavu na druhem konci serioveho kabelu

    /// obsluha prichozich dat pokud je v prichozim bufferu vice
    void checkBuffer();
};

//const int Stimulator::DISTRIBUTION_LED[LEDS_COUNT];

#endif //STDCR_MONITOR_SERIAL_COMM_H
