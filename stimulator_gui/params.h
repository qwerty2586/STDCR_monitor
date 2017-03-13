/**
 * V tomto hlavickovem souboru, prosim o udrzovani pouzivanych konstant napric projektem
 * do budoucna je zde planovana featura nacitani techto konstant z XML souboru
 */


#ifndef STDCR_MONITOR_PARAMS_H
#define STDCR_MONITOR_PARAMS_H


#include <QString>
#include <QStringList>

///velikost okna
extern const int MAIN_WIDTH;
extern const int MAIN_HEIGHT;
extern const int MAX_LEDS;
extern const QString SCHEMAS_DIR;
extern const QString SCHEMAS_EXTENSION;
extern const QString TEXT_MS;
extern const QString TEXT_PERCENT;
extern const QString TEXT_CELSIUS;
extern const QString TEXT_MV;
extern const QString TEXT_YEARS;
extern const QString TEXT_KG;
extern const QString TEXT_CM;
extern const int MIN_MS;
extern const int MAX_MS;
extern const int MIN_P;
extern const int MAX_P;
extern const int MIN_COUNT;
extern const int MAX_COUNT;
extern const int MIN_AGE;
extern const int MAX_AGE;
extern const int MIN_BODY_WEIGHT;
extern const int MAX_BODY_WEIGHT;
extern const int MIN_BODY_HEIGHT;
extern const int MAX_BODY_HEIGHT;


extern const int WINDOWS_MAX_COM;
extern const QString LINUX_DEV_DIR;
extern const QStringList LINUX_DEV_MASK;
extern const QString FILESERVER_DIR;


#endif //STDCR_MONITOR_PARAMS_H
