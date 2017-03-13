
#include "sysname.h"
#include <sysinfoapi.h>
#include <rpc.h>
#include <QDebug>
#define INFO_BUFFER_SIZE 32767

const QString& getSystemName() {

    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;


    GetComputerName( infoBuf, &bufCharCount );
    QString *name =new QString();
    int i=0;
    while(infoBuf[i]!='\000' && i<INFO_BUFFER_SIZE) {
        name->append((char)infoBuf[i]);
        i++;
    }
    return *name;
};
