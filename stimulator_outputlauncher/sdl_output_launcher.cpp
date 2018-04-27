#include "sdl_output_launcher.h"
#include <QDebug>
#include <iostream>


void SdlOutputLauncher::doStartSdlOutput(QString configfile) {
    if (started) return;

    QString sudo = "/usr/bin/sudo";
    QString working_dir = "/home/qwerty/SDL_output";
    QString sudo_params = "./SDL_output -x 600 -y 480 -w --config output_test/config.xml";

    child.setWorkingDirectory(working_dir);
    child.start(sudo, sudo_params.split(" "));
    this->started = true;


}

void SdlOutputLauncher::doStopSdlOutput() {
    if (!started) return;

    // tady je zapotrebi  killnout child process od sudo a ne sudo samotne protoze jinak z SDL_output vznikne orphan
    QProcess killer;
    QString sudo = "/usr/bin/sudo";
    QString kill_command = "pkill SDL_output";  // pouzijeme systemovy zabijak
    killer.start(sudo,kill_command.split(" "));
    killer.waitForFinished();
    started = false;

}
