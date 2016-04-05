#ifndef STDCR_MONITOR_SERIAL_COMM_H
#define STDCR_MONITOR_SERIAL_COMM_H

#include <QObject>


class SerialComm : public QObject {
Q_OBJECT

public:
    explicit SerialComm(const QString filename = 0);

    void setFile(const QString filename);

    void fileConnect();

    88typedef
    commPacket = {};
private:
    QThread

            boolean
    connected,
    opened;
    QFile comm_file;


};

#endif //STDCR_MONITOR_SERIAL_COMM_H
