//
// Created by qwerty on 4. 4. 2017.
//

#include "sdl_output_launcher.h"
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void SdlOutputLauncher::doStartSdlOutput(QString configfile) {
    if (started) return;
    const char* sdloutput_params = "\"~/SDL_output/SDL_output -x 600 -y 480 -w --config ~/output_test/config.xml\"";
    const char** sudo_params = (const char*[]){"/usr/bin/sudo",sdloutput_params,NULL};
    child_id = fork();
    if (child_id==0) {

    }




}

void SdlOutputLauncher::doStopSdlOutput() {
    if (!started) return;
    kill(child_id,SIGKILL);

}
