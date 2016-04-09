#include <QFile>
#include "stimulator.h"


Stimulator::Stimulator(const QString filename) {
    opened = false;
    connected = false;
    m_port = new QextSerialPort(QextSerialPort::EventDriven);
    connect(m_port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
    if (filename != 0) setFile(filename);

}

void Stimulator::setFile(const QString filename) {
    m_port->setPortName(filename);

}

void Stimulator::fileConnect() {
    if (m_port->open(QIODevice::ReadWrite) == true) {
        opened = true;
    }
    else {
        opened = false;
    }
}

void Stimulator::onReadyRead() {
    QByteArray bytes;
    int bytesLength = m_port->bytesAvailable();
    bytes.resize(bytesLength);
    m_port->read(bytes.data(), bytes.size());
    buffer.append(bytes);
    if (buffer.size() >= PACKET_SIZE)
        checkBuffer();
}

void Stimulator::onDsrChanged(bool status) {
    if (status)
        deviceOn = true;
    else
        deviceOn = false;
}

void Stimulator::checkBuffer() {

}




