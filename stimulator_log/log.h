

#ifndef STIMULATOR_CONTROL_LOG_H
#define STIMULATOR_CONTROL_LOG_H


#include <string>
#include <QString>
#include <QDebug>


class Log {
public:
    enum Levels {
        ERROR =0,
        WARN,
        INFO,
        DEBUG,
        VERBOSE
    };

    static int const debug_level = DEBUG;


    static void e(const QString &log_message) { log(ERROR , log_message);};

    static void w(const QString &log_message) { log(WARN , log_message);};

    static void i(const QString &log_message) { log(INFO , log_message);};

    static void d(const QString &log_message) { log(DEBUG , log_message);} ;

    static void v(const QString &log_message) { log(VERBOSE , log_message);};

private:
    void static log(Levels lev,const QString &log_message) { if (debug_level <= lev) print(log_message);};
    void static print(const QString &text) {
        qDebug() << qPrintable(text);
    };
};

#endif //STIMULATOR_CONTROL_LOG_H
