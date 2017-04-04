#ifndef STIMULATOR_CONTROL_SDL_OUTPUT_LAUNCHER_H
#define STIMULATOR_CONTROL_SDL_OUTPUT_LAUNCHER_H

#include <QObject>

class SdlOutputLauncher : public QObject {
    Q_OBJECT
public:


public slots:
    void doStartSdlOutput(QString configfile);
    void doStopSdlOutput();

private:
    bool started = false;
    int child_id = 0;


};

#endif //STIMULATOR_CONTROL_SDL_OUTPUT_LAUNCHER_H
