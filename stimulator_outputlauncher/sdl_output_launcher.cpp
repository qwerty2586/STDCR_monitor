#include "sdl_output_launcher.h"
#include <QDebug>
#include <stimulator_gui/params.h>
#include <QtCore/QDir>
#include <QtWidgets/QMessageBox>
#include <QtCore/QCoreApplication>


void SdlOutputLauncher::doStartSdlOutput(QString configfile) {
    if (started || !Params::sdlEnabled()) return;

    QString sudo = SUDO_PATH;
    QString working_dir = FILESERVER_DIR;
    QFileInfo sdl_output_executable(QCoreApplication::applicationDirPath(),SDL_OUTPUT);

    if (!sdl_output_executable.exists())  {
        QMessageBox box;
        box.setText("SDL_output is not in same directory as stimulator_control, cant proceed");
        box.setDefaultButton(QMessageBox::Ok);
        box.exec();
        return;
    }
    QStringList sudo_params(sdl_output_executable.absoluteFilePath());
    // size of window
    sudo_params.append("-x");
    sudo_params.append(QString::number(Params::sdlWidth()));
    sudo_params.append("-y");
    sudo_params.append(QString::number(Params::sdlHeight()));
    if (Params::sdlFullscreen()) {
        // fullscreen
        sudo_params.append("-f");
    } else {
        // window
        sudo_params.append("-w");
    }
    if (Params::sdlDebug()) {
        sudo_params.append("-t");
    }

    // config file
    sudo_params.append("-c");
    if (configfile.isEmpty()) configfile = AUTOSAVE_FILE;
    QFileInfo info(QDir(FILESERVER_DIR),configfile);
    if (!info.exists()) {
        QMessageBox box;
        box.setText("Couldn't find file "+configfile+" with configuration");
        box.setDefaultButton(QMessageBox::Ok);
        box.exec();
        return;
    }
    sudo_params.append(configfile);

    child.setWorkingDirectory(working_dir);
    child.start(sudo, sudo_params);
    this->started = true;


}

void SdlOutputLauncher::doStopSdlOutput() {
    if (!started || !Params::sdlEnabled()) return;

    // tady je zapotrebi  killnout child process od sudo a ne sudo samotne protoze jinak z SDL_output vznikne orphan
    QProcess killer;
    QString sudo = SUDO_PATH;
    QString kill_command = "pkill "+SDL_OUTPUT;  // pouzijeme systemovy zabijak
    killer.start(sudo,kill_command.split(" "));
    killer.waitForFinished();
    started = false;

}
