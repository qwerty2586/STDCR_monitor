#include "serials.h"
#include <3rdparty/qextserialport/qextserialenumerator.h>

QStringList listOfAvailableSerials() {
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    QStringList r;
            foreach (QextPortInfo info, ports) {
            r.append(info.portName);
        }
    return r;
};