#include "params.h"

const int MAIN_WIDTH = 500;
const int MAIN_HEIGHT = 400;
const int MAX_LEDS = 8;
const QString SCHEMAS_DIR = "./schemas";
const QString SCHEMAS_EXTENSION = ".xml";
const QString FILESERVER_DIR = "output_data/";
const QString AUTOSAVE_FILE = ".autosave.xml";
const QString OUTPUTS_DIR = FILESERVER_DIR;
const QString OUTPUTS_EXTENSION = ".xml";
const QString SDL_OUTPUT = "SDL_output";
const QString SUDO_PATH = "/usr/bin/sudo";
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
const int WINDOWS_MAX_COM = 20;;
const QString LINUX_DEV_DIR = "/dev";
const QStringList LINUX_DEV_MASK = QString("ttyU*;ttyA*;ttyS*;rfcomm*").split(';');

;

namespace Params {
    namespace {
        const QString APP_ORG = "zcu";
        const QString APP_NAME = "stimulator_control";



        static void setValue(const QString &key, const QVariant value) {
            QSettings(QSettings::NativeFormat, QSettings::UserScope, APP_ORG, APP_NAME).setValue(key, value);
        }

        static QVariant value(const QString &key, const QVariant default_value) {
            return QSettings(QSettings::NativeFormat, QSettings::UserScope, APP_ORG, APP_NAME).value(key, default_value);
        }

        const QString KEY_SDL_ENABLED = "sdl_enabled";
        const bool DEFAULT_SDL_ENABLED = false;

        const QString KEY_SDL_FULLSCREEN = "sdl_fullscreen";       
        const bool DEFAULT_SDL_FULLSCREEN = false;

        const QString KEY_SDL_DEBUG = "sdl_debug";
        const bool DEFAULT_SDL_DEBUG = false;

        const QString KEY_SDL_WIDTH = "sdl_width";
        const int DEFAULT_SDL_WIDTH  = 800;

        const QString KEY_SDL_HEIGHT = "sdl_height";
        const int DEFAULT_SDL_HEIGHT = 600;

    }
    void setSdlEnabled(bool enabled) {setValue(KEY_SDL_ENABLED,enabled);}
    bool sdlEnabled() { return value(KEY_SDL_ENABLED,DEFAULT_SDL_ENABLED).toBool();    }

    void setSdlFullscreen(bool fullscreen) {setValue(KEY_SDL_FULLSCREEN,fullscreen);};
    bool sdlFullscreen() {return value(KEY_SDL_FULLSCREEN,DEFAULT_SDL_FULLSCREEN).toBool();  };

    void setSdlDebug(bool debug) {setValue(KEY_SDL_DEBUG,debug);};
    bool sdlDebug() {return value(KEY_SDL_DEBUG,DEFAULT_SDL_DEBUG).toBool();  };

    void setSdlWidth(int width){setValue(KEY_SDL_WIDTH,width);};
    int sdlWidth(){ return value(KEY_SDL_WIDTH,DEFAULT_SDL_WIDTH).toInt();};

    void setSdlHeight(int height){setValue(KEY_SDL_HEIGHT,height);};
    int sdlHeight(){ return value(KEY_SDL_HEIGHT,DEFAULT_SDL_HEIGHT).toInt();};

}

