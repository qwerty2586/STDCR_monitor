
#include "sysname.h"
#include <sys/utsname.h>
#include <QDebug>
#include <QStringBuilder>

const QString &getSystemName() {
    utsname uname_r;
    uname(&uname_r);
    auto name = new QString(uname_r.nodename);
    name->append(" ");
    name->append(uname_r.sysname);
    name->append(" ");
    name->append(uname_r.release);

    return *name;
};
