#ifndef STDCR_MONITOR_SERIALS_H
#define STDCR_MONITOR_SERIALS_H

#include <QStringList>



/// funkce vrati seznam nazvu seriovych portu
QStringList listOfAvailableSerials();

extern QList<int> listOfAvailableBaudSpeeds;
#endif //STDCR_MONITOR_SERIALS_H
