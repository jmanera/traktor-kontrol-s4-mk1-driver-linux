//
// Created by aspgems on 17/11/22.
//
#include <iostream>
#include <cstdlib>
#include "includes/RtMidiHelper.h"
#include "includes/EvDevHelper.h"
#include <spdlog/spdlog.h>
using namespace std;

RtMidiHelper *rtmidi_helper;
EvDevHelper *evdev_helper;

void shutdown_application(int signum){
    EvDevHelper::shutdown_buttons_leds();
    rtmidi_helper->close_input_port();
    rtmidi_helper->close_output_port();
    spdlog::warn("Exiting application with code: {0}", signum);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    signal(SIGTERM,shutdown_application);
    signal(SIGKILL,shutdown_application);
    signal(SIGSEGV,shutdown_application);
    spdlog::set_level(spdlog::level::info);
    evdev_helper = new EvDevHelper();
    rtmidi_helper = new RtMidiHelper();

    RtMidiHelper::show_midi_information(rtmidi_helper);
    evdev_helper->initialize_buttons_leds();
    evdev_helper->read_events_from_device(rtmidi_helper->pMidiOut);
    exit( EXIT_SUCCESS );
}
