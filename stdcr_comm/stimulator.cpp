#include <QFile>
#include "stimulator.h"


Stimulator::Stimulator(const QString filename) {
    opened = false;
    m_port = new QextSerialPort(QextSerialPort::EventDriven);
    connect(m_port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
    if (filename != 0) setFile(filename);

}

void Stimulator::setFile(const QString filename) {
    m_port->setPortName(filename);
    fileConnect();

}

void Stimulator::fileConnect() {
    bool lastState = opened;
    opened = m_port->open(QIODevice::ReadWrite);
    if (lastState != opened) emit connected(opened);
}

void Stimulator::onReadyRead() {
    QByteArray bytes;
    int bytesLength = (int) m_port->bytesAvailable();
    bytes.resize(bytesLength);
    m_port->read(bytes.data(), bytes.size());
    buffer.append(bytes);
    if (buffer.size() >= PACKET_SIZE)
        checkBuffer();
}

void Stimulator::onDsrChanged(bool status) {
    deviceOn = status;
}

void Stimulator::checkBuffer() {
    while (buffer.size() >= PACKET_SIZE) {
        incoming_type = buffer[1];
        int incoming_length = buffer[0] && 0x3F; // vynulujeme prvni dva bity
        if (incoming_length == 63) {
            incoming_data.append(buffer.mid(2, 62)); // pripojime data zpravy
            buffer.remove(0, PACKET_SIZE);
        } else {
            incoming_data.append(buffer.mid(2, incoming_length));
            buffer.remove(0, PACKET_SIZE);
            emit incomingMessage(incoming_type, incoming_data); // odesleme signal
            incoming_data.clear();
        }
    }
}




