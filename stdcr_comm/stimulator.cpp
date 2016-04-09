#include "stimulator.h"


Stimulator::Stimulator(const QString filename) {
    opened = false;
    connected = false;
    if (filename) setFile(filename);
}

void Stimulator::setFile(const QString filename) {
    comm_file = QFile(filename);
    if (comm_file) opened = true;
    else opened = false;
}

void Stimulator::fileConnect() {
    connected = true;
}


