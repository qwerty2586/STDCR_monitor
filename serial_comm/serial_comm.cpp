#include "serial_comm.h"


SerialComm::SerialComm(const QString filename) {
    opened = false;
    connected = false;
    setFile(filename);
}

void SerialComm::setFile(const QString filename) {
    comm_file = QFile(filename);
    if (comm_file) opened = true;
    else opened = false;
}

void SerialComm::fileConnect() {
    connected = true;
}


