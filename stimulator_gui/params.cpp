#include "params.h"

const int MAIN_WIDTH = 500;
const int MAIN_HEIGHT = 400;
const int MAX_LEDS = 8;
const QString SCHEMAS_DIR = "./schemas";
const QString SCHEMAS_EXTENSION = ".xml";
const QString TEXT_MS = "[ms]";
const QString TEXT_PERCENT = "[\%]";
const QString TEXT_CELSIUS = QString::fromUtf8("[°C]");
const QString TEXT_MV = "[mV]";
const QString TEXT_YEARS = "yr";
const QString TEXT_KG = "[kg]";
const QString TEXT_CM = "[cm]";
const int MIN_MS = 0;
const int MAX_MS = 30000;
const int MIN_P = 0;
const int MAX_P = 100;
const int MIN_COUNT = 0;
const int MAX_COUNT = 100;
const int MIN_AGE = 1;
const int MAX_AGE = 150;
const int MIN_BODY_WEIGHT = 1;
const int MAX_BODY_WEIGHT = 250;
const int MIN_BODY_HEIGHT = 1;
const int MAX_BODY_HEIGHT = 250;

// komunikace
const int WINDOWS_MAX_COM = 12;;
const QString LINUX_DEV_DIR = "/dev";
const QStringList LINUX_DEV_MASK = QString("ttyU*;ttyA*;ttyS*;rfcomm*").split(';');