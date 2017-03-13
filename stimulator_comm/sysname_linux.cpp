
#include "sysname.h"
#include <sys/utsname.h>
#include <QDebug>
#include <QStringBuilder>

const QString &getSystemName() {
    utsname uname_r;
    uname(&uname_r);
    QString *name = new QString(QString(uname_r.nodename) % " " % QString(uname_r.sysname) %
                                " " % QString(uname_r.release));
    return *name;

};
