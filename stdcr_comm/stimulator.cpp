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
        int incoming_length = buffer[0] & 0x3F; // vynulujeme prvni dva bity
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

void Stimulator::portDisconnect() {
    bool lastState = opened;
    if (m_port->isOpen()) {
        m_port->close();
        incoming_data.clear();
        buffer.clear();
    }
    opened = false;
    if (lastState != opened) emit connected(opened);
}

bool Stimulator::isPortConnected() {
    return opened;
}

void Stimulator::sendMessage(char messageType) {
    sendMessage(messageType, QByteArray());
}

void Stimulator::sendMessage(char messageType, char messageData) {
    sendMessage(messageType, QByteArray(1, messageData));
}

void Stimulator::sendMessage(char messageType, int messageData) {
    int16_t data = messageData;
    char low = data & 0xFF;
    char high = (data >> 8) & 0xFF; //rozdeleni na dva bajty
    QByteArray array;
    array.append(high);
    array.append(low);
    sendMessage(messageType, array);
}

void Stimulator::sendMessage(char messageType, QByteArray messageData) {
    QByteArray output;
    while (messageData.size() >= 63) {
        output.clear();
        output.append(63); // delka
        output.append(messageType);
        output.append(messageData.left(62));
        messageData.remove(0, 62);
        m_port->write(output);
    }
    output.clear();
    output.append(messageData.size());
    output.append(messageType);
    output.append(messageData);
    if (output.size() < PACKET_SIZE) {
        output.append(QByteArray(PACKET_SIZE - output.size(), '\0')); // doplnime nulami
    }
    m_port->write(output);


}
















