#include "serials.h"
#include <QFileInfo>


QStringList listOfAvailableSerials() {
    QStringList r;
    QString com;


    for (int i = 1; i <= 12; i++) {
        com = "COM" + QString::number(i);
        r.append(com);
    }

    return r;
};
