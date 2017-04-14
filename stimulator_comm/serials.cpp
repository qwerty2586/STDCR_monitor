//
// Created by qwerty on 14. 4. 2017.
//

#include <QList>
#include "../3rdparty/qextserialport/qextserialport.h"

QList<int> listOfAvailableBaudSpeeds =  {
#if defined(Q_OS_UNIX) || defined(qdoc)
BAUD50,                //POSIX ONLY
    BAUD75,                //POSIX ONLY
    BAUD134,              //POSIX ONLY
    BAUD150,              //POSIX ONLY
    BAUD200,              //POSIX ONLY
    BAUD1800,            //POSIX ONLY
#  if defined(B76800) || defined(qdoc)
    BAUD76800,          //POSIX ONLY
#  endif
#  if (defined(B230400) && defined(B4000000)) || defined(qdoc)
    BAUD230400,        //POSIX ONLY
    BAUD460800,        //POSIX ONLY
    BAUD500000,        //POSIX ONLY
    BAUD576000,        //POSIX ONLY
    BAUD921600,        //POSIX ONLY
    BAUD1000000,      //POSIX ONLY
    BAUD1152000,      //POSIX ONLY
    BAUD1500000,      //POSIX ONLY
    BAUD2000000,      //POSIX ONLY
    BAUD2500000,      //POSIX ONLY
    BAUD3000000,      //POSIX ONLY
    BAUD3500000,      //POSIX ONLY
    BAUD4000000,      //POSIX ONLY
#  endif
#endif //Q_OS_UNIX
#if defined(Q_OS_WIN) || defined(qdoc)
BAUD14400,          //WINDOWS ONLY
        BAUD56000,          //WINDOWS ONLY
        BAUD128000,        //WINDOWS ONLY
        BAUD256000,        //WINDOWS ONLY
#endif  //Q_OS_WIN
BAUD110,
BAUD300,
BAUD600,
BAUD1200,
BAUD2400,
BAUD4800,
BAUD9600,
BAUD19200,
BAUD38400,
BAUD57600,
BAUD115200
};
