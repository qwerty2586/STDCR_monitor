#include "serials.h"
#include <QDir>
#include <stdcr_monitor/params.h>


QStringList listOfAvailableSerials() {
    // zase nefunguje enumenator takze hledame rucne
    QStringList r;
    QDir dir(LINUX_DEV_DIR, "", QDir::Name, QDir::System);
    dir.setNameFilters(LINUX_DEV_MASK);
    r.append(dir.entryList());
    return r;
};