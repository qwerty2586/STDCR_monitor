//
// Created by qwerty on 4. 4. 2017.
//

#include "sdl_output_launcher.h"
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void SdlOutputLauncher::doStartSdlOutput(QString configfile) {
    if (started) return;
    char* sdloutput_params = "\"~/SDL_output/SDL_output -x 600 -y 480 -w --config ~/output_test/config.xml\"";
    char** arr = new char*[3];
    arr[0] = "/usr/bin/sudo";
    arr[1] = sdloutput_params;
    arr[2] = NULL;
    //arr = {"/usr/bin/sudo",sdloutput_params,NULL};
    //const char** sudo_params = (const char*[]){"/usr/bin/sudo",sdloutput_params,NULL};
    child_id = fork();
 //   const char* sdloutput_params = "\"~/SDL_output/SDL_output -x 600 -y 480 -w --config ~/output_test/config.xml\"";
  //  const char** sudo_params = (const char*[]){"/usr/bin/sudo",sdloutput_params,NULL};
 //   child_id = fork();
    if (child_id==0) {
        setsid();
        execv(arr[0],arr);
        exit(0);
    }




}

void SdlOutputLauncher::doStopSdlOutput() {
    if (!started) return;
  //  kill(child_id,SIGKILL);

}
