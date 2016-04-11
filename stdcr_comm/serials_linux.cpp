#include "serials.h"
#include <QDir>


QStringList listOfAvailableSerials() {

    QStringList r;
    QDir dir("/dev");
    QStringList filters;
    filters.push_back("ttyU*");
    filters.push_back("ttyA*");
    filters.push_back("ttyS*");
    filters.push_back("rfcomm*");
    dir.setNameFilters(filters);
    r.append(dir.entryList());
    return r;
};